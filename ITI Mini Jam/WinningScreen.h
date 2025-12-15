#pragma once
#include <SFML/Graphics.hpp>
#include "UI.h"
#include "SoundManager.h"

class WinningScreen
{
private:
    sf::Sprite bgSprite;
    sf::Texture bgTexture;

    UIButton replayBtn;
    UIButton menuBtn;

    sf::Font winFont;
    sf::Text congratsText;

    sf::Texture tReplay, tReplayHover;
    sf::Texture tMenu, tMenuHover;

    SoundManager* soundMgr = nullptr;

public:
    WinningScreen(float width, float height, SoundManager* sm);

    // return:
    // 0 = nothing
    // 1 = replay
    // 2 = main menu
    int update(sf::RenderWindow& window, const sf::Event& ev);
    void draw(sf::RenderWindow& window);
};
