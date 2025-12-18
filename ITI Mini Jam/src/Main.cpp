#include <SFML/Graphics.hpp>

#include "../include/Game.h"
#include "../include/Menu.h"
#include "../include/OptionsMenu.h"
#include "../include/RainSystem.h"
#include "../include/GameOverScreen.h"
#include "../include/WinningScreen.h"
#include "../include/SoundManager.h"
#include "../include/PauseMenu.h"

using namespace sf;

int main()
{
    auto mode = VideoMode::getDesktopMode();
    float WIDTH = static_cast<float>(mode.width);
    float HEIGHT = static_cast<float>(mode.height);

    RenderWindow window(mode, "ESC CTRL", Style::None);
    window.setFramerateLimit(60);

    window.clear(Color::Black);
    window.display();

    SoundManager soundMgr;
    soundMgr.playMusic("menu", true);

    Menu menu(WIDTH, HEIGHT, &soundMgr);
    OptionsMenu options(WIDTH, HEIGHT, &soundMgr);
    PauseMenu pauseMenu(WIDTH, HEIGHT, &soundMgr);
    GameOverScreen gameOver(WIDTH, HEIGHT);
    WinningScreen winningScreen(WIDTH, HEIGHT, &soundMgr);

    Game* game = nullptr;

    enum GameState {
        MENU_STATE,
        PLAYING_STATE,
        PAUSED_STATE,
        OPTIONS_STATE,
        GAMEOVER_STATE,
        WINNING_STATE
    };

    GameState gameState = MENU_STATE;
    GameState previousState = MENU_STATE;

    Clock dtClock;

    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)
            {
                if (gameState == PLAYING_STATE)
                {
                    previousState = gameState;
                    gameState = PAUSED_STATE;
                }
                else if (gameState == PAUSED_STATE)
                {
                    gameState = PLAYING_STATE;
                }
            }

            if (gameState == OPTIONS_STATE)
            {
                window.setView(window.getDefaultView());
                if (options.update(window, e) == 1)
                    gameState = previousState;
            }
            else if (gameState == GAMEOVER_STATE && game)
            {
                window.setView(window.getDefaultView());
                if (gameOver.update(window, e))
                {
                    delete game;
                    game = new Game(WIDTH, HEIGHT, &soundMgr);
                    gameState = PLAYING_STATE;
                }
            }
            else if (gameState == WINNING_STATE && game)
            {
                window.setView(window.getDefaultView());
                int res = winningScreen.update(window, e);

                if (res == 1) {
                    delete game;
                    game = new Game(WIDTH, HEIGHT, &soundMgr);
                    gameState = PLAYING_STATE;
                }
                else if (res == 2) {
                    delete game;
                    game = nullptr;
                    gameState = MENU_STATE;
                }
            }
            else if (gameState == PAUSED_STATE && game)
            {
                window.setView(window.getDefaultView());
                int res = pauseMenu.update(window, e);

                if (res == 1) gameState = PLAYING_STATE;
                else if (res == 2) {
                    delete game;
                    game = nullptr;
                    gameState = MENU_STATE;
                }
            }
        }

        float dt = dtClock.restart().asSeconds();
        window.clear(Color::Black);

        if (gameState == MENU_STATE)
        {
            int r = menu.update(window);
            menu.draw(window);

            if (r == 1) {
                game = new Game(WIDTH, HEIGHT, &soundMgr);
                gameState = PLAYING_STATE;
            }
            else if (r == 2) {
                previousState = MENU_STATE;
                gameState = OPTIONS_STATE;
            }
            else if (r == 3)
                window.close();
        }
        else if (gameState == PLAYING_STATE && game)
        {
            bool died = game->update(dt);
            game->draw(window);

            if (died) gameState = GAMEOVER_STATE;
            else if (game->hasWon()) gameState = WINNING_STATE;
        }
        else if (gameState == PAUSED_STATE && game)
        {
            window.setView(game->getCamera());
            game->draw(window);

            window.setView(window.getDefaultView());
            pauseMenu.draw(window);
        }
        else if (gameState == OPTIONS_STATE)
        {
            menu.draw(window);
            options.draw(window);
        }
        else if (gameState == GAMEOVER_STATE && game)
        {
            window.setView(game->getCamera());
            game->draw(window);

            window.setView(window.getDefaultView());
            gameOver.draw(window, window.getDefaultView());
        }
        else if (gameState == WINNING_STATE && game)
        {
            window.setView(game->getCamera());
            game->draw(window);

            window.setView(window.getDefaultView());
            winningScreen.draw(window);
        }

        window.display();
    }

    delete game;
    return 0;
}
