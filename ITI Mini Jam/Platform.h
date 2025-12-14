#pragma once
#include <SFML/Graphics.hpp>

class Platform {
public:
    Platform(float x, float y, float width, float height, sf::Color color = sf::Color::White);
    Platform(const sf::Texture& texture, float x, float y, float width, float height);

    void setMovement(float moveX, float moveY, float speed);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;

    //Helper function to help us determine how much we moved this specific frame
    sf::Vector2f getCurrentShift() const;

private:
    bool useSprite = false;
    sf::RectangleShape rect;
    sf::Sprite sprite;

    sf::Vector2f startPos;
    sf::Vector2f range;
    float speed = 0.f;
    float progress = 0.f;

    // NEW: Stores the delta movement of the current frame
    sf::Vector2f currentShift = { 0.f, 0.f };
};