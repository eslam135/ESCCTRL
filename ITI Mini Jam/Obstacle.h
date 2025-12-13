#pragma once

#include <SFML/Graphics.hpp>

class Obstacle
{
public:
    Obstacle(
        float x = 0.f,
        float y = 0.f,
        float width = 80.f,
        float height = 120.f,
        sf::Color color = sf::Color(180, 40, 40, 220)
    );

    Obstacle(
        const sf::Texture& texture,
        float x,
        float y,
        float width,
        float height
    );

    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;

private:
    bool useSprite = false;

    sf::RectangleShape rect;
    sf::Sprite sprite;
};
