#pragma once
#include "RainSystem.h"
#include <SFML/Graphics.hpp> 

class Level2 {
public:
	static void setupSpikeRain(RainSystem& rain, const sf::Texture& spikeTex);
};
