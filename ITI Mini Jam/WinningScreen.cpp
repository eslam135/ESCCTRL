#include "WinningScreen.h"
#include <iostream>

using namespace sf;
using namespace std;

WinningScreen::WinningScreen(float width, float height, SoundManager* sm)
{
    soundMgr = sm;

    // Background
    if (!bgTexture.loadFromFile("Assets/PauseMenu/PauseMenuBG.png"))
        cerr << "Failed to load Winning BG\n";

    bgSprite.setTexture(bgTexture);
    bgSprite.setScale(
        width / bgTexture.getSize().x,
        height / bgTexture.getSize().y
    );

    float cx = width / 2.f;
    float cy = height / 2.f;

    replayBtn = UIButton({ 422.f, 142.f }, { cx, cy - 60.f });
    menuBtn = UIButton({ 422.f, 142.f }, { cx, cy + 120.f });

    // ---------- CONGRATULATIONS TEXT ----------
    if (!winFont.loadFromFile("Assets/Fonts/MyFont.ttf"))
        cerr << "Failed to load win font\n";

    congratsText.setFont(winFont);
    congratsText.setString("CONGRATULATIONS!");
    congratsText.setCharacterSize(64);
    congratsText.setFillColor(Color::Black);

    FloatRect tb = congratsText.getLocalBounds();
    congratsText.setOrigin(tb.width / 2.f, tb.height / 2.f);
    congratsText.setPosition(cx, cy - 240.f);
    // -----------------------------------------

    // Replay
    tReplay.loadFromFile("Assets/Buttons/ReplayButton.png");
    tReplayHover.loadFromFile("Assets/Buttons/ReplayButton_hover.png");
    replayBtn.rect.setTexture(&tReplay);

    // Menu
    tMenu.loadFromFile("Assets/Buttons/exit.png");
    tMenuHover.loadFromFile("Assets/Buttons/exit_hover.png");
    menuBtn.rect.setTexture(&tMenu);
}

int WinningScreen::update(RenderWindow& window, const Event& ev)
{
    Vector2i mp = Mouse::getPosition(window);

    // Hover
    replayBtn.rect.setTexture(
        replayBtn.contains(mp) ? &tReplayHover : &tReplay
    );
    menuBtn.rect.setTexture(
        menuBtn.contains(mp) ? &tMenuHover : &tMenu
    );

    if (ev.type == Event::MouseButtonPressed &&
        ev.mouseButton.button == Mouse::Left)
    {
        if (replayBtn.contains(mp)) {
            if (soundMgr) soundMgr->playSFX("button_click");
            return 1;
        }
        if (menuBtn.contains(mp)) {
            if (soundMgr) soundMgr->playSFX("button_click");
            return 2;
        }
    }
    return 0;
}

void WinningScreen::draw(RenderWindow& window)
{
    window.draw(bgSprite);
    window.draw(congratsText);   // NEW
    replayBtn.draw(window);
    menuBtn.draw(window);
}
