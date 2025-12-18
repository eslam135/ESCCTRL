#include "../include/WinningScreen.h"
#include <iostream>

using namespace sf;
using namespace std;

WinningScreen::WinningScreen(float width, float height, SoundManager* sm)
{
    soundMgr = sm;

    // -------- Background --------
    if (!bgTexture.loadFromFile("Assets/PauseMenu/PauseMenuBG.png"))
        cerr << "Failed to load Winning BG\n";

    bgSprite.setTexture(bgTexture);
    bgSprite.setScale(
        width / bgTexture.getSize().x,
        height / bgTexture.getSize().y
    );

    float cx = width / 2.f;
    float cy = height / 2.f;

    // -------- Buttons --------
    replayBtn = UIButton({ 422.f, 142.f }, { cx, cy - 60.f });
    menuBtn = UIButton({ 422.f, 142.f }, { cx, cy + 120.f });

    // -------- Congratulations Text --------
    if (!winFont.loadFromFile("Assets/Fonts/MyFont.ttf"))
        cerr << "Failed to load win font\n";

    congratsText.setFont(winFont);
    congratsText.setString("CONGRATULATIONS!");
    congratsText.setCharacterSize(64);
    congratsText.setFillColor(Color::Black);

    FloatRect tb = congratsText.getLocalBounds();
    congratsText.setOrigin(tb.width / 2.f, tb.height / 2.f);
    congratsText.setPosition(cx, cy - 240.f);

    // -------- Load Textures --------
    tReplay.loadFromFile("Assets/Buttons/ReplayButton.png");
    tReplayHover.loadFromFile("Assets/Buttons/ReplayButton_hover.png");
    replayBtn.rect.setTexture(&tReplay);

    tMenu.loadFromFile("Assets/Buttons/exit.png");
    tMenuHover.loadFromFile("Assets/Buttons/exit_hover.png");
    menuBtn.rect.setTexture(&tMenu);
}

int WinningScreen::update(RenderWindow& window, const Event& ev)
{
    Vector2i mousePos = Mouse::getPosition(window);

    // -------- Hover logic --------
    if (replayBtn.contains(mousePos)) {
        if (tReplayHover.getSize().x)
            replayBtn.rect.setTexture(&tReplayHover);
    }
    else if (tReplay.getSize().x) {
        replayBtn.rect.setTexture(&tReplay);
    }

    if (menuBtn.contains(mousePos)) {
        if (tMenuHover.getSize().x)
            menuBtn.rect.setTexture(&tMenuHover);
    }
    else if (tMenu.getSize().x) {
        menuBtn.rect.setTexture(&tMenu);
    }

    // -------- Click logic --------
    if (ev.type == Event::MouseButtonPressed &&
        ev.mouseButton.button == Mouse::Left)
    {
        if (replayBtn.contains(mousePos)) {
            if (soundMgr) soundMgr->playSFX("button_click");
            return 1; // Replay
        }
        if (menuBtn.contains(mousePos)) {
            if (soundMgr) soundMgr->playSFX("button_click");
            return 2; // Back to Menu
        }
    }

    return 0;
}

void WinningScreen::draw(RenderWindow& window)
{
    window.draw(bgSprite);
    window.draw(congratsText);
    replayBtn.draw(window);
    menuBtn.draw(window);
}
