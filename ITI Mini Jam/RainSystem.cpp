#include "RainSystem.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace sf;
using namespace std;

// --- Constructor for menu rain ---
RainSystem::RainSystem(int count, float W, float H)
    : WIDTH(W), HEIGHT(H)
{
    srand(static_cast<unsigned>(time(nullptr)));
    for (int i = 0; i < count; i++) {
        RainDrop rd;
        rd.shape.setRadius(2.5f);
        rd.shape.setFillColor(Color(173, 216, 230, 180));
        rd.shape.setPosition(rand() % static_cast<int>(WIDTH + 500), rand() % static_cast<int>(HEIGHT));
        rd.speed = 300 + rand() % 200;
        drops.push_back(rd);
    }
}

// --- Main menu rain ---
void RainSystem::update(float dt)
{
    for (auto& rd : drops)
    {
        rd.shape.move(rd.speed / 2.f * -dt, rd.speed * dt);
        if (rd.shape.getPosition().y > HEIGHT)
            rd.shape.setPosition(rand() % static_cast<int>(WIDTH + 500), -10.f);
    }
}

void RainSystem::draw(RenderWindow& window)
{
    for (auto& rd : drops)
        window.draw(rd.shape);
}

// --- Helper to spawn or recycle a spike ---
void RainSystem::spawnSpike(Spike& s, const SpikeRainZone& zone)
{
    float x = zone.minX + static_cast<float>(rand()) / RAND_MAX * (zone.maxX - zone.minX);
    s.sprite.setPosition(x, -20.f);
    s.speed = 300 + rand() % 200;
    s.active = true;
}

// --- Spike rain update ---
void RainSystem::updateSpikeRain(float dt, float playerX)
{
    for (auto& zone : spikeZones)
    {
        if (!zone.triggered && playerX >= zone.triggerX) {
            zone.triggered = true;
            zone.elapsed = 0.f;
            zone.spawnTimer = 0.f;
        }

        if (zone.triggered && zone.elapsed < zone.duration) {
            zone.elapsed += dt;
            zone.spawnTimer += dt;

            // Spawn a burst when timer exceeds current spawnInterval
            if (zone.spawnTimer >= zone.spawnInterval) {
                zone.spawnTimer = 0.f;

                // Randomize burst count and next interval
                zone.burstCount = zone.minBurstCount + rand() % (zone.maxBurstCount - zone.minBurstCount + 1);
                zone.spawnInterval = zone.minSpawnInterval + static_cast<float>(rand()) /
                    (RAND_MAX / (zone.maxSpawnInterval - zone.minSpawnInterval));

                for (int i = 0; i < zone.burstCount; ++i) {
                    for (auto& s : spikes) {
                        if (!s.active) {
                            spawnSpike(s, zone);
                            break; // one spike per iteration
                        }
                    }
                }
            }
        }
    }

    // Move all active spikes
    for (auto& s : spikes) {
        if (s.active) {
            s.sprite.move(0.f, s.speed * dt);
            if (s.sprite.getPosition().y > HEIGHT) s.active = false;
        }
    }
}

// --- Draw spikes ---
void RainSystem::drawSpikeRain(RenderWindow& window)
{
    for (auto& s : spikes)
        if (s.active) window.draw(s.sprite);
}

// --- Collision check ---
bool RainSystem::checkSpikeCollision(const FloatRect& playerBounds)
{
    for (auto& s : spikes) {
        if (s.active && playerBounds.intersects(s.sprite.getGlobalBounds()))
            return true;
    }
    return false;
}

// --- Add spike rain zone with randomized burst range and interval range ---
void RainSystem::addSpikeRainZone(const sf::Texture& tex, float triggerX, float duration, float minX, float maxX,
    int minBurstCount, int maxBurstCount, float minInterval, float maxInterval)
{
    spikeTex = tex;

    SpikeRainZone zone;
    zone.triggerX = triggerX;
    zone.duration = duration;
    zone.minX = minX;
    zone.maxX = maxX;
    zone.minBurstCount = minBurstCount;
    zone.maxBurstCount = maxBurstCount;
    zone.minSpawnInterval = minInterval;
    zone.maxSpawnInterval = maxInterval;
    zone.elapsed = 0.f;
    zone.spawnTimer = 0.f;
    zone.triggered = false;

    spikeZones.push_back(zone);

    // Pre-allocate spikes
    for (int i = 0; i < maxBurstCount * 10; ++i) {
        Spike s;
        s.sprite.setTexture(spikeTex);
        s.sprite.setOrigin(spikeTex.getSize().x / 2.f, spikeTex.getSize().y / 2.f);
        s.sprite.setScale(0.2f, 0.2f);
        s.active = false;
        spikes.push_back(s);
    }
}
