#include "PauseMenu.h"
#include <iostream>

using namespace sf;

PauseMenu::PauseMenu(float width, float height, SoundManager* sm)
{
    soundMgr = sm;

    // -------- Background --------
    if (!bgTexture.loadFromFile("Assets/PauseMenu/PauseMenuBG.png"))
        std::cerr << "Failed to load pause background\n";

    bgSprite.setTexture(bgTexture);
    bgSprite.setScale(
        width / bgTexture.getSize().x,
        height / bgTexture.getSize().y
    );

    float cx = width / 2.f;
    float cy = height / 2.f;

    // -------- Buttons --------
    resumeBtn = UIButton({ 422.f, 142.f }, { cx, cy - 100.f });
    exitBtn = UIButton({ 367.f, 141.f }, { cx, cy + 100.f });

    // -------- Load textures --------
    if (tResume.loadFromFile("Assets/Buttons/ResumeButton.png"))
        resumeBtn.rect.setTexture(&tResume);

    tResumeHover.loadFromFile("Assets/Buttons/ResumeButton_hover.png");

    if (tExit.loadFromFile("Assets/Buttons/ExitButton.png"))
        exitBtn.rect.setTexture(&tExit);

    tExitHover.loadFromFile("Assets/Buttons/ExitButton_hover.png");
}

int PauseMenu::update(RenderWindow& window, const Event& ev)
{
    Vector2i mousePos = Mouse::getPosition(window);

    // -------- Hover logic --------
    if (resumeBtn.contains(mousePos)) {
        if (tResumeHover.getSize().x)
            resumeBtn.rect.setTexture(&tResumeHover);
    }
    else if (tResume.getSize().x) {
        resumeBtn.rect.setTexture(&tResume);
    }

    if (exitBtn.contains(mousePos)) {
        if (tExitHover.getSize().x)
            exitBtn.rect.setTexture(&tExitHover);
    }
    else if (tExit.getSize().x) {
        exitBtn.rect.setTexture(&tExit);
    }

    // -------- Click logic --------
    if (ev.type == Event::MouseButtonPressed &&
        ev.mouseButton.button == Mouse::Left)
    {
        if (resumeBtn.contains(mousePos)) {
            if (soundMgr) soundMgr->playSFX("button_click");
            return 1; // Resume
        }
        if (exitBtn.contains(mousePos)) {
            if (soundMgr) soundMgr->playSFX("button_click");
            return 2; // Exit to menu
        }
    }

    return 0;
}

void PauseMenu::draw(RenderWindow& window)
{
    window.draw(bgSprite);
    resumeBtn.draw(window);
    exitBtn.draw(window);
}
