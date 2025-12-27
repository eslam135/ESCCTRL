#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Platform.h"
#include "Obstacle.h"

class LevelDesign {
public:
    struct SpikeRainZone {
        float triggerX;
        float duration;
        int count;
        float minX, maxX;
    };

    static void buildLevel(
        float height,
        std::vector<Platform>& platforms,
        std::vector<Obstacle>& obstacles,
        const sf::Texture& platformTex,
        const sf::Texture& obstacleTex,
        const sf::Texture& circlethornsTex,
        const sf::Texture& shortThornsTexture,  
        const sf::Texture& mediumThornsTex,     
        const sf::Texture& longThornsTex

        
    );

    static void buildProps(
        float width,
        float height,
        std::vector<sf::Sprite>& trees,
        std::vector<sf::Sprite>& leaves,
        const std::vector<sf::Texture>& propTextures
    );
};