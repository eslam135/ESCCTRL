#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Platform.h"
#include "Obstacle.h"

class LevelDesign {
public:
    static void buildLevel(
        float height,
        std::vector<Platform>& platforms,
        std::vector<Obstacle>& obstacles,
        const sf::Texture& platformTex,
        const sf::Texture& obstacleTex
    );

    static void buildProps(
        float width,
        float height,
        std::vector<sf::Sprite>& trees,
        std::vector<sf::Sprite>& leaves,
        const std::vector<sf::Texture>& propTextures
    );
};