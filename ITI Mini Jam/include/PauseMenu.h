#pragma once
#include <SFML/Graphics.hpp>
#include "UI.h"
#include "SoundManager.h"

class PauseMenu {
public:
    PauseMenu(float width, float height, SoundManager* sm = nullptr);

    int update(sf::RenderWindow& window, const sf::Event& ev);
    void draw(sf::RenderWindow& window);

private:
    SoundManager* soundMgr = nullptr;

    // Background
    sf::Texture bgTexture;
    sf::Sprite bgSprite;

    // Buttons
    UIButton resumeBtn;
    UIButton exitBtn;

    // Button textures
    sf::Texture tResume;
    sf::Texture tResumeHover;

    sf::Texture tExit;
    sf::Texture tExitHover;
};
