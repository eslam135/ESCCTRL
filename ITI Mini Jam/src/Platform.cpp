#include "../include/Platform.h"
#include <cmath>

using namespace sf;

Platform::Platform(float x, float y, float width, float height, Color color) {
    useSprite = false;
    rect.setSize({ width, height });
    rect.setPosition(x, y);
    rect.setFillColor(color);
    startPos = { x, y };
}

Platform::Platform(const Texture& texture, float x, float y, float width, float height) {
    useSprite = true;
    sprite.setTexture(texture);
    Vector2u texSize = texture.getSize();
    sprite.setScale(width / texSize.x, height / texSize.y);
    sprite.setPosition(x, y);
    startPos = { x, y };
}

void Platform::setMovement(float moveX, float moveY, float spd) {
    range = { moveX, moveY };
    speed = spd;
}

void Platform::update(float dt) {
    if (speed == 0.f) {
        currentShift = { 0.f, 0.f };
        return;
    }

    // 1. Get Old Position
    Vector2f oldPos = useSprite ? sprite.getPosition() : rect.getPosition();

    // 2. Calculate New Position
    progress += speed * dt;
    float offsetFactor = std::sin(progress);
    Vector2f newPos = startPos + (range * offsetFactor);

    // 3. Set New Position
    if (useSprite) sprite.setPosition(newPos);
    else rect.setPosition(newPos);

    // 4. Calculate Shift (Delta) so we can push the player later
    currentShift = newPos - oldPos;
}

void Platform::draw(RenderWindow& window) {
    if (useSprite) window.draw(sprite);
    else window.draw(rect);
}

FloatRect Platform::getBounds() const {
    return useSprite ? sprite.getGlobalBounds() : rect.getGlobalBounds();
}

// Implement the getter
Vector2f Platform::getCurrentShift() const {
    return currentShift;
}