#include "../include/Level2.h"
#include <cstdlib> 
#include <ctime>   

void Level2::setupSpikeRain(RainSystem& rain, const sf::Texture& spikeTex)
{
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned>(time(nullptr)));
        seeded = true;
    }

    // Zone 1: Early game rain
    //rain.addSpikeRainZone(
    //    spikeTex,
    //    700.f,    // triggerX
    //    100.f,    // duration
    //    1000.f,   // minX
    //    1400.f,   // maxX
    //    3, 7, 1.f, 3.f
    //);

    // Zone 2: Mid game rain
    rain.addSpikeRainZone(
        spikeTex,
        2000.f,
        4.f,
        2000.f,
        2600.f,
        3, 7,
        2.f, 4.f
    );

    // Zone 3
    rain.addSpikeRainZone(
        spikeTex,
        15300.f,  // Trigger X (Start of rain run)
        10.f,     // Duration (Long enough to cross)
        15000.f,  // MinX
        17000.f,  // MaxX
        3, 7,    // Burst Count (Heavy rain!)
        0.5f, 1.5f // Fast intervals
    );
}