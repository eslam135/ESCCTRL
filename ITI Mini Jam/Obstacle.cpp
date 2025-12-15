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

    sprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);

    sprite.setPosition(x + width / 2.f, y + height / 2.f);

    startY = y + height / 2.f;
}

void Obstacle::update(float dt, float playerX) {
    if (type == ROTATING) {
        sprite.rotate(rotationSpeed * dt);
    }
    else if (type == FALLING) {
        if (hasFallen) {
            sprite.move(0, fallSpeed * dt);
            fallSpeed += 1500.f * dt;
        }
        else {
            if (std::abs(sprite.getPosition().x - playerX) < 100.f) {
                hasFallen = true;
                fallSpeed = 200.f;
            }
        }
    }
}

void Obstacle::draw(RenderWindow& window) {
    if (type == FROG_ITEM || type == GRAVITY_SWITCH) return;

    window.draw(sprite);
}

FloatRect Obstacle::getBounds() const {
    FloatRect b = sprite.getGlobalBounds();

    if (type != GRAVITY_SWITCH && type != FROG_ITEM) {
        b.left += 10;
        b.width -= 20;
        b.top += 10;
        b.height -= 20;
    }

    return b;
}