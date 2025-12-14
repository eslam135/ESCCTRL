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

    // --- Moving platform features ---
    void setMoving(float leftBound, float rightBound, float speed);
    void update(float dt); // call this each frame

private:
    bool useSprite = false;

    sf::RectangleShape rect;
    sf::Sprite sprite;

    // Moving properties
    bool isMoving = false;
    float speed = 0.f;
    float leftBound = 0.f;
    float rightBound = 0.f;
    int direction = 1; // 1 = right, -1 = left
};