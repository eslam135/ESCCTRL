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

void Platform::setMoving(
    MoveAxis axis, float minB, float maxB,
    float spd, bool triggered,
    MoveMode mode
)
{
    isMoving = true;
    moveAxis = axis;
    minBound = minB;
    maxBound = maxB;
    speed = spd;
    direction = 1;
    isTriggered = triggered;
    isActive = !triggered;
    moveMode = mode;
}


void Platform::update(float dt)
{
    deltaX = deltaY = 0.f;

    if (!isMoving || !isActive)
        return;

    float movement = speed * direction * dt;

    if (moveAxis == MoveAxis::Horizontal)
    {
        deltaX = movement;

        if (useSprite) sprite.move(movement, 0.f);
        else rect.move(movement, 0.f);

        float x = useSprite ? sprite.getPosition().x : rect.getPosition().x;
        float width = getBounds().width;

        if (x < minBound)
        {
            if (useSprite)
                sprite.setPosition(minBound, sprite.getPosition().y);
            else
                rect.setPosition(minBound, rect.getPosition().y);

            if (moveMode == MoveMode::PingPong)
                direction = 1;
            else
                isActive = false; // OneWay stops here

            deltaX = 0.f;
        }
        else if (x + width > maxBound)
        {
            if (useSprite)
                sprite.setPosition(maxBound - width, sprite.getPosition().y);
            else
                rect.setPosition(maxBound - width, rect.getPosition().y);

            if (moveMode == MoveMode::PingPong)
                direction = -1;
            else
                isActive = false;

            deltaX = 0.f;
        }
    }
    else // Vertical
    {
        deltaY = movement;

        if (useSprite) sprite.move(0.f, movement);
        else rect.move(0.f, movement);

        float y = useSprite ? sprite.getPosition().y : rect.getPosition().y;
        float height = getBounds().height;

        if (y < minBound)
        {
            if (useSprite)
                sprite.setPosition(sprite.getPosition().x, minBound);
            else
                rect.setPosition(rect.getPosition().x, minBound);

            if (moveMode == MoveMode::PingPong)
                direction = 1;
            else
                isActive = false;

            deltaY = 0.f;
        }
        else if (y + height > maxBound)
        {
            if (useSprite)
                sprite.setPosition(sprite.getPosition().x, maxBound - height);
            else
                rect.setPosition(rect.getPosition().x, maxBound - height);

            if (moveMode == MoveMode::PingPong)
                direction = -1;
            else
                isActive = false;

            deltaY = 0.f;
        }
    }
}



void Platform::activate()
{
    if (isTriggered)
        isActive = true;
}