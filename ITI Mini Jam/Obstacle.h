#pragma once
#include <SFML/Graphics.hpp>

class Obstacle {
public:
    // The 4 types of obstacles/interactables
    enum Type { STATIC, ROTATING, FALLING, GRAVITY_SWITCH };

    Obstacle(const sf::Texture& texture, float x, float y, float width, float height, Type type = STATIC);

    void update(float dt, float playerX);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;

    // Helper so Game.cpp knows if this is a spike (die) or a switch (flip gravity)
    Type getType() const { return type; }

private:
    bool useSprite = true;
    sf::Sprite sprite;
    Type type;

    float rotationSpeed = 200.f;
    bool hasFallen = false;
    float fallSpeed = 0.f;
    float startY;
};