#pragma once

#include <SFML/Graphics.hpp>

class Platform
{
public:
    // Rectangle version
    Platform(
        float x = 0.f,
        float y = 0.f,
        float width = 100.f,
        float height = 20.f,
        sf::Color color = sf::Color::White
    );

    // Sprite version
    Platform(
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
