#include "Obstacle.h"

using namespace sf;

Obstacle::Obstacle(
    float x,
    float y,
    float width,
    float height,
    Color color
)
{
    useSprite = false;

    rect.setSize({ width, height });
    rect.setOrigin(width / 2.f, height / 2.f);
    rect.setPosition(x, y);
    rect.setFillColor(color);
}

Obstacle::Obstacle(
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

    sprite.setOrigin(
        texSize.x / 2.f,
        texSize.y / 2.f
    );

    sprite.setPosition(x, y);
}

void Obstacle::draw(RenderWindow& window)
{
    if (useSprite)
        window.draw(sprite);
    else
        window.draw(rect);
}

FloatRect Obstacle::getBounds() const
{
    return useSprite
        ? sprite.getGlobalBounds()
        : rect.getGlobalBounds();
}
