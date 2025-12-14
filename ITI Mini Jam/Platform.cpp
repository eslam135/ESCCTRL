#include "Platform.h"

using namespace sf;

Platform::Platform(
    float x,
    float y,
    float width,
    float height,
    Color color
)
{
    useSprite = false;

    rect.setSize({ width, height });
    rect.setPosition(x, y);
    rect.setFillColor(color);
}

Platform::Platform(
    const Texture& texture,
    float x,
    float y,
    float width,
    float height
)
{
    useSprite = true;

    sprite.setTexture(texture);

    Vector2u texSize = texture.getSize();
    sprite.setScale(
        width / texSize.x,
        height / texSize.y
    );

    sprite.setPosition(x, y);
}

void Platform::draw(RenderWindow& window)
{
    if (useSprite)
        window.draw(sprite);
    else
        window.draw(rect);
}

FloatRect Platform::getBounds() const
{
    return useSprite
        ? sprite.getGlobalBounds()
        : rect.getGlobalBounds();
}

// --- Moving platform methods ---

void Platform::setMoving(float left, float right, float spd)
{
    isMoving = true;
    leftBound = left;
    rightBound = right;
    speed = spd;
    direction = 1;
}

void Platform::update(float dt)
{
    if (!isMoving) return;

    if (useSprite)
        sprite.move(speed * direction * dt, 0.f);
    else
        rect.move(speed * direction * dt, 0.f);

    // Reverse direction if out of bounds
    float x = useSprite ? sprite.getPosition().x : rect.getPosition().x;
    float width = useSprite ? sprite.getGlobalBounds().width : rect.getSize().x;

    if (x < leftBound)
    {
        if (useSprite) sprite.setPosition(leftBound, sprite.getPosition().y);
        else rect.setPosition(leftBound, rect.getPosition().y);
        direction = 1;
    }
    else if (x + width > rightBound)
    {
        if (useSprite) sprite.setPosition(rightBound - width, sprite.getPosition().y);
        else rect.setPosition(rightBound - width, rect.getPosition().y);
        direction = -1;
    }
}