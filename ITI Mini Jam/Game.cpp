#include "Game.h"
#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include "LevelDesign.h" 
#include "Level2.h"

using namespace sf;
using namespace std;

Game::Game(float W, float H, SoundManager* sm)
    : WIDTH(W), HEIGHT(H),
    rain(80, W, H),
    bg(5, W * 10000.f, H, { 0.f, 0.f , 12.f, 18.f , 60.f }, 0),
    BGground(1, W * 10000.f, H, { 0.f }, 5),
    ground(0.f, H - 200.f, W * 10000.f - 100.f, 200.f, Color(0, 255, 0, 0))
{
    // --- 1. SETUP ---
    soundMgr = sm;
    player.soundMgr = sm;
    WORLD_RIGHT = WIDTH * 10000.f;

    camera.setSize(WIDTH, HEIGHT);
    camera.setCenter(WIDTH / 2.f, HEIGHT / 2.f);

    // --- 2. LOAD TEXTURES ---
    if (!platformTexture.loadFromFile("Assets/Platforms/Pltfrm1.png"))
        std::cerr << "Couldn't load platform texture\n";
    if (!obstacleTexture.loadFromFile("Assets/Spikes/Spikes2.png"))
        std::cerr << "Couldn't load obstacle texture\n";
    if (!thornsTexture.loadFromFile("Assets/Thorns/Thorns.png"))
        std::cerr << "Couldn't load thorns texture\n"; //The thorns I added
    if (!spikeTex.loadFromFile("Assets/Spikes/Spike.png"))
        std::cerr << "Couldn't load spike texture\n";

    // --- 3. BUILD LEVEL ---
    LevelDesign::buildLevel(HEIGHT, platforms, obstacles, platformTexture, obstacleTexture, thornsTexture);
    Level2::setupSpikeRain(rain, spikeTex);

    // --- 4. PROPS ---
    Texture t;
    if (t.loadFromFile("Assets/Props/Leaves1.png")) propTextures.push_back(t);
    if (t.loadFromFile("Assets/Props/Tree.png"))    propTextures.push_back(t);

    LevelDesign::buildProps(WIDTH, HEIGHT, treesProp, leavesProp, propTextures);
}

bool Game::update(float dt)
{
    // --- NEW: Update Cooldown Timer ---
    if (switchCooldown > 0.f) {
        switchCooldown -= dt;
    }

    player.updateMovement();
    player.onGround = false;

    for (auto& p : platforms) p.update(dt);
    for (auto& o : obstacles) o.update(dt, player.getPosition().x);
    
    rain.updateSpikeRain(dt, player.getPosition().x);

    if (rain.checkSpikeCollision(player.getGlobalBounds())) {
        return true; // player dies
    }

    CollisionManager::resolveAll(player, platforms, ground, player.velY, player.onGround);

    player.updateAnimation();
    syncRunSound();

    // 5. CHECK DEATH CONDITIONS
    if (checkObstacleCollision()) return true;

    // Bottom Border
    if (player.getPosition().y > HEIGHT + 100) return true;

    // Top Border (Only if reversed)
    if (player.isGravityReversed && player.getPosition().y < -100) return true;

    float direction = 0;
    if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) direction = -1;
    else if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) direction = 1;

    if (direction != 0) bg.update(dt, direction, 3, bg.layerCount);
    bg.update(dt, 1, 2, 3);

    float px = player.getPosition().x;
    px = max(px, WORLD_LEFT + WIDTH / 2.f);
    px = min(px, WORLD_RIGHT - WIDTH / 2.f);
    camera.setCenter(px, HEIGHT / 2.f);

    return false;
}

bool Game::checkObstacleCollision()
{
    FloatRect playerBounds = player.getGlobalBounds();

    for (size_t i = 0; i < obstacles.size(); ++i) {
        if (playerBounds.intersects(obstacles[i].getBounds())) {

            // CASE 1: GRAVITY SWITCH
            if (obstacles[i].getType() == Obstacle::GRAVITY_SWITCH) {

               
                if (switchCooldown <= 0.f) {
                    player.setGravityReversed(!player.isGravityReversed);
                    if (soundMgr) soundMgr->playSFX("jump");

                    switchCooldown = 0.5f;
                }


                obstacles.erase(obstacles.begin() + i);
                return false;
            }

            // CASE 2: DEADLY TRAP
            if (soundMgr) soundMgr->stopSFX("run");
            return true;
        }
    }
    return false;
}

void Game::draw(RenderWindow& window)
{
    window.setView(camera);
    bg.draw(window);
    ground.draw(window);

    for (auto& p : treesProp) if (isVisible(p)) window.draw(p);

    BGground.draw(window);

    for (auto& plat : platforms) plat.draw(window);
    for (auto& o : obstacles) o.draw(window);

    rain.drawSpikeRain(window);

    player.draw(window);

    for (auto& p : leavesProp) if (isVisible(p)) window.draw(p);
}

void Game::syncRunSound()
{
    if (!soundMgr) return;
    bool running = player.isRunningOnGround();
    if (running) soundMgr->playSFX("run", true);
    else soundMgr->stopSFX("run");
}

bool Game::isVisible(const sf::Sprite& sprite)
{
    sf::FloatRect camRect(
        camera.getCenter().x - WIDTH / 2.f,
        camera.getCenter().y - HEIGHT / 2.f,
        WIDTH,
        HEIGHT
    );
    return sprite.getGlobalBounds().intersects(camRect);
}

void Game::reset()
{
    player.hitbox.setPosition(300.f, 300.f);
    player.velY = 0.f;
    player.onGround = false;
    player.setGravityReversed(false);
    player.currentState = Player::IDLE;
    player.currentFrame = 0;
    player.sprite.setTexture(player.tIdle);
    player.sprite.setTextureRect(IntRect(0, 0, player.frameW, player.frameH));
    camera.setCenter(WIDTH / 2.f, HEIGHT / 2.f);

    // Reset timer
    switchCooldown = 0.f;

    syncRunSound();

    platforms.clear();
    obstacles.clear();
    LevelDesign::buildLevel(HEIGHT, platforms, obstacles, platformTexture, obstacleTexture, thornsTexture);
}