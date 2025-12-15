#pragma once

#include <SFML/Graphics.hpp>
#include "UI.h"

class GameOverScreen {
public:
    GameOverScreen(float width, float height);

    bool update(sf::RenderWindow& window, const sf::Event& ev);
    void draw(sf::RenderWindow& window, const sf::View& view);

private:
    sf::RectangleShape overlay;

    sf::Font font;

    UIButton restartButton;

    // Replay button textures
    sf::Texture tReplay;
    sf::Texture tReplayHover;
};
