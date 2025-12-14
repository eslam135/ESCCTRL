#include "Obstacle.h"
#include <cmath>
#include <iostream>

using namespace sf;

Obstacle::Obstacle(const Texture& texture, float x, float y, float width, float height, Type t)
    : type(t)
{
    sprite.setTexture(texture);
    Vector2u texSize = texture.getSize();
    sprite.setScale(width / texSize.x, height / texSize.y);

    // CRITICAL: Set origin to center so rotating blades spin correctly
    // and flipping/scaling works as expected.
    sprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);

    // Adjust position: Since origin is center, we shift by half width/height
    sprite.setPosition(x + width / 2.f, y + height / 2.f);

    startY = y + height / 2.f;
}

void Obstacle::update(float dt, float playerX) {
    if (type == ROTATING) {
        // Spin constant speed
        sprite.rotate(rotationSpeed * dt);
    }
    else if (type == FALLING) {
        if (hasFallen) {
            // Apply Gravity
            sprite.move(0, fallSpeed * dt);
            fallSpeed += 1500.f * dt;
        }
        else {
            // Trigger fall if player is close horizontally
            if (std::abs(sprite.getPosition().x - playerX) < 100.f) {
                hasFallen = true;
                fallSpeed = 200.f; // Initial downward push
            }
        }
    }
}

void Obstacle::draw(RenderWindow& window) {
    window.draw(sprite);
}

FloatRect Obstacle::getBounds() const {
    FloatRect b = sprite.getGlobalBounds();

    // CRITICAL HITBOX LOGIC:
    // If it is a deadly trap (STATIC, ROTATING, FALLING), we shrink the hitbox
    // so the game feels "fair" (you don't die on invisible pixels).
    //
    // If it is a GRAVITY_SWITCH, we leave it full size (or could even make it bigger)
    // to ensure the player definitely hits it.

    if (type != GRAVITY_SWITCH) {
        b.left += 10;
        b.width -= 20;
        b.top += 10;
        b.height -= 20;
    }

    return b;
}