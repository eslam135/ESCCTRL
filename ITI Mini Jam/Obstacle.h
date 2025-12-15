#pragma once
#include <SFML/Graphics.hpp>

class Obstacle {
public:
    // Added FROG_ITEM to the list
    enum Type { STATIC, ROTATING, FALLING, GRAVITY_SWITCH, FROG_ITEM };

    Obstacle(const sf::Texture& texture, float x, float y, float width, float height, Type type = STATIC);

    void update(float dt, float playerX);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;

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