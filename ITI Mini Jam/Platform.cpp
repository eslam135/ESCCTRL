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
