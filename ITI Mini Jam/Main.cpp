#include <SFML/Graphics.hpp>

#include "Game.h"
#include "Menu.h"
#include "OptionsMenu.h"
#include "RainSystem.h"
#include "GameOverScreen.h"
#include "SoundManager.h"
#include "PauseMenu.h"

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

    Game* game = nullptr;

    enum GameState {
        MENU_STATE,
        PLAYING_STATE,
        PAUSED_STATE,
        OPTIONS_STATE,
        GAMEOVER_STATE
    };

    GameState gameState = MENU_STATE;
    GameState previousState = MENU_STATE;

    Clock dtClock;

    RectangleShape fadeOverlay(Vector2f(WIDTH, HEIGHT));
    fadeOverlay.setFillColor(Color::Black);
    float fadeAlpha = 255.f;
    float fadeSpeed = 150.f;

    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

            // ESC toggle pause
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

            // ----- OPTIONS -----
            if (gameState == OPTIONS_STATE)
            {
                window.setView(window.getDefaultView());

                int res = options.update(window, e);
                if (res == 1)
                    gameState = previousState;
            }

            // ----- GAME OVER -----
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

            // ----- PAUSE MENU -----
            else if (gameState == PAUSED_STATE && game)
            {
                window.setView(window.getDefaultView());
                int res = pauseMenu.update(window, e);

                if (res == 1)        gameState = PLAYING_STATE;      // Resume
                else if (res == 2) {                                 // Main Menu
                    delete game;
                    game = nullptr;
                    gameState = MENU_STATE;
                }
            }
        }

        float dt = dtClock.restart().asSeconds();
        window.clear(Color::Black);

        // ---------- DRAW ----------
        if (gameState == MENU_STATE)
        {
            int menuResult = menu.update(window);
            menu.draw(window);

            if (menuResult == 1) // PLAY
            {
                if (!game)
                    game = new Game(WIDTH, HEIGHT, &soundMgr);
                gameState = PLAYING_STATE;
            }
            else if (menuResult == 2) // OPTIONS
            {
                previousState = MENU_STATE;
                gameState = OPTIONS_STATE;
            }
            else if (menuResult == 3) // EXIT
                window.close();
        }
        else if (gameState == PLAYING_STATE && game)
        {
            bool died = game->update(dt);
            game->draw(window);

            if (died)
                gameState = GAMEOVER_STATE;
        }
        else if (gameState == PAUSED_STATE && game)
        {
            // Draw world
            window.setView(game->getCamera());
            game->draw(window);

            // Draw UI
            window.setView(window.getDefaultView());
            pauseMenu.draw(window);
        }

        else if (gameState == OPTIONS_STATE)
        {
            window.setView(window.getDefaultView());

            menu.draw(window);
            options.draw(window);
        }
        else if (gameState == GAMEOVER_STATE && game)
        {
            // World
            window.setView(game->getCamera());
            game->draw(window);

            // UI
            window.setView(window.getDefaultView());
            gameOver.draw(window, window.getDefaultView());

        }

        // ----- Fade-in -----
        if (fadeAlpha > 0.f)
        {
            fadeAlpha -= fadeSpeed * dt;
            if (fadeAlpha < 0.f) fadeAlpha = 0.f;
            fadeOverlay.setFillColor(Color(0, 0, 0, static_cast<Uint8>(fadeAlpha)));
            window.draw(fadeOverlay);
        }

        window.display();
    }

    delete game;
    return 0;
}
