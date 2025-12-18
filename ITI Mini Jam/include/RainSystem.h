#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct RainDrop {
    sf::CircleShape shape;
    float speed;
};

struct Spike {
    sf::Sprite sprite;
    float speed;
    bool active;
};

struct SpikeRainZone {
    float triggerX;
    float duration;
    float elapsed = 0.f;
    float minX, maxX;
    bool triggered = false;

    // Burst parameters
    float minSpawnInterval = 0.5f; // randomized interval range
    float maxSpawnInterval = 1.f;
    int minBurstCount = 3;          // randomized burst range
    int maxBurstCount = 7;

    float spawnInterval = 0.5f;     // current interval until next burst
    int burstCount = 5;             // current burst count
    float spawnTimer = 0.f;         // timer to accumulate delta time
};

class RainSystem {
public:
    std::vector<RainDrop> drops;
    std::vector<Spike> spikes;
    std::vector<SpikeRainZone> spikeZones;
    sf::Texture spikeTex;

    float WIDTH, HEIGHT;

    RainSystem(int count, float W, float H);

    // --- Main menu rain ---
    void update(float dt);
    void draw(sf::RenderWindow& window);

    // --- Spike rain ---
    void setupSpikeRain(const sf::Texture& tex, int count, float triggerX, float duration, float minX, float maxX);
    void updateSpikeRain(float dt, float playerX);
    void drawSpikeRain(sf::RenderWindow& window);
    bool checkSpikeCollision(const sf::FloatRect& playerBounds);
    void addSpikeRainZone(const sf::Texture& tex, float triggerX, float duration, float minX, float maxX,
        int minBurstCount, int maxBurstCount, float minInterval, float maxInterval);

private:
    void spawnSpike(Spike& s, const SpikeRainZone& zone);
};

