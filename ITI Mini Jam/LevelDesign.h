#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Platform.h"
#include "Obstacle.h"

class LevelDesign {
public:
    LevelDesign(float gameWidth, float gameHeight);

    void loadLevel(std::vector<Platform>& platforms, std::vector<Obstacle>& obstacles, sf::Texture& platformTexture, sf::Texture& obstacleTexture);

private:
    float WIDTH;
    float HEIGHT;
};
