#include "Level2.h"
#include <cstdlib> // for rand
#include <ctime>   // for time

void Level2::setupSpikeRain(RainSystem& rain, const sf::Texture& spikeTex)
{
    // Seed the RNG once
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned>(time(nullptr)));
        seeded = true;
    }

    // Zone 1: passes ranges instead of fixed numbers
    rain.addSpikeRainZone(
        spikeTex,
        700.f,    // triggerX
        100.f,    // duration
        1000.f,   // minX
        1400.f,   // maxX
        3,        // minBurstCount
        7,        // maxBurstCount
        1.f,      // minSpawnInterval
        3.f       // maxSpawnInterval
    );

    // Zone 2
    rain.addSpikeRainZone(
        spikeTex,
        2000.f,
        4.f,
        2000.f,
        2600.f,
        3, 7, 2.f, 4.f
    );

    // Add more zones similarly if needed
}
