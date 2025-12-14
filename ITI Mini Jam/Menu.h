#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "SoundManager.h"
#include "UI.h"

class Menu {
public:
    sf::Sprite bg;

    std::vector<sf::Texture> bgFrames;
    size_t currentFrame = 0;
    int frameDirection = 1;          
    float frameTime = 0.1f;
    sf::Clock frameClock;
    static constexpr int FRAME_STEP = 3;

    UIButton btnStart, btnOptions, btnExit;
    sf::Texture tStart, tStartHover;
    sf::Texture tOptions, tOptionsHover;
    sf::Texture tExit, tExitHover;

    sf::Texture tTitle;
    sf::Sprite sTitle;

    SoundManager* soundMgr = nullptr;

    Menu(float WIDTH, float HEIGHT, SoundManager* sm = nullptr);

    int update(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

private:
    void loadAnimatedBackground(const std::string& folder,
        float WIDTH, float HEIGHT);
    void updateBackground();
};
