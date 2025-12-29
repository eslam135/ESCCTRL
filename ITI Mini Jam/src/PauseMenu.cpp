#include "../include/PauseMenu.h"
#include <iostream>

using namespace sf;

PauseMenu::PauseMenu(float width, float height, SoundManager* sm)
{
    soundMgr = sm;

    if (!bgTexture.loadFromFile("Assets/PauseMenu/PauseMenuBG.png"))
        std::cerr << "Failed to load pause background\n";

    bgSprite.setTexture(bgTexture);
    bgSprite.setScale(
        width / bgTexture.getSize().x,
        height / bgTexture.getSize().y
    );

    float scaleX = width / 1920.f;
    float scaleY = height / 1080.f;

    float cx = width / 2.f;
    float cy = height / 2.f;


    resumeBtn = UIButton(
        { 422.f * scaleX, 142.f * scaleY },
        { cx, cy - (100.f * scaleY) }
    );

    exitBtn = UIButton(
        { 367.f * scaleX, 141.f * scaleY },
        { cx, cy + (100.f * scaleY) }
    );

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

    if (ev.type == Event::MouseButtonPressed &&
        ev.mouseButton.button == Mouse::Left)
    {
        if (resumeBtn.contains(mousePos)) {
            if (soundMgr) soundMgr->playSFX("button_click");
            return 1; 
        }
        if (exitBtn.contains(mousePos)) {
            if (soundMgr) soundMgr->playSFX("button_click");
            return 2; 
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