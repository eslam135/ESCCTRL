#include "GameOverScreen.h"
#include <iostream>

using namespace sf;
using namespace std;

GameOverScreen::GameOverScreen(float width, float height)
{
    // -------- Overlay --------
    overlay.setSize({ width, height });
    overlay.setFillColor(Color(0, 0, 0, 180));

    // -------- Font & Title --------
    if (!font.loadFromFile("Assets/Fonts/MyFont.ttf"))
        cerr << "Warning: MyFont.ttf not found for GameOver screen\n";

    float cx = width / 2.f;
    float cy = height / 2.f;

    // -------- Replay Button --------
    restartButton = UIButton({ 426.f, 170.f }, { cx, cy });

    if (tReplay.loadFromFile("Assets/Buttons/ReplayButton.png"))
        restartButton.rect.setTexture(&tReplay);

    tReplayHover.loadFromFile("Assets/Buttons/ReplayButton_hover.png");
    
}

bool GameOverScreen::update(RenderWindow& window, const Event& ev)
{
    Vector2i mousePos = Mouse::getPosition(window);
    mousePos.x += 90;
    // -------- Hover --------
    if (restartButton.contains(mousePos)) {
        if (tReplayHover.getSize().x)
            restartButton.rect.setTexture(&tReplayHover);
    }
    else if (tReplay.getSize().x) {
        restartButton.rect.setTexture(&tReplay);
    }

    // -------- Click --------
    if (ev.type == Event::MouseButtonPressed &&
        ev.mouseButton.button == Mouse::Left)
    {
        if (restartButton.contains(mousePos)) {
            return true; // restart game
        }
    }

    return false;
}

void GameOverScreen::draw(RenderWindow& window, const View& view)
{
    Vector2f size = view.getSize();
    Vector2f center = view.getCenter();

    overlay.setSize(size);
    overlay.setOrigin(size.x / 2.f, size.y / 2.f);
    overlay.setPosition(center);

	restartButton.rect.setPosition(center);

    window.draw(overlay);
    restartButton.draw(window);
}
