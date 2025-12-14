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

    enum class MoveAxis {
        Horizontal,
        Vertical
    };

    enum class MoveMode {
        PingPong,
        OneWay
    };

    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;

    // --- Moving platform features ---
    void setMoving(
        MoveAxis axis,
        float minBound,
        float maxBound,
        float speed,
        bool triggered = false,
        MoveMode mode = MoveMode::PingPong
    );


    void update(float dt);

    float getDeltaX() const { return deltaX; }
    float getDeltaY() const { return deltaY; }
    bool isMovingPlatform() const { return isMoving; }

    void activate(); // for triggered platforms

private:
    bool useSprite = false;

    sf::RectangleShape rect;
    sf::Sprite sprite;

    bool isMoving = false;
    bool isTriggered = false;
    bool isActive = true;

    MoveAxis moveAxis = MoveAxis::Horizontal;
    MoveMode moveMode = MoveMode::PingPong;

    float speed = 0.f;
    float minBound = 0.f;
    float maxBound = 0.f;
    int direction = 1;

    float deltaX = 0.f;
    float deltaY = 0.f;
};