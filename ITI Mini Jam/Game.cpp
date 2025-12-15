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
    soundMgr = sm;
    player.soundMgr = sm;
    WORLD_RIGHT = WIDTH * 10000.f;

    camera.setSize(WIDTH, HEIGHT);
    camera.setCenter(WIDTH / 2.f, HEIGHT / 2.f);

    if (!platformTexture.loadFromFile("Assets/Platforms/Pltfrm1.png")) cerr << "Error: Platform tex missing\n";
    if (!obstacleTexture.loadFromFile("Assets/Spikes/Spikes2.png")) cerr << "Error: Spike tex missing\n";
    if (!thornsTexture.loadFromFile("Assets/Thorns/Thorns.png")) cerr << "Error: Thorns tex missing\n";
    if (!spikeTex.loadFromFile("Assets/Spikes/Spike.png")) cerr << "Error: Spike rain tex missing\n";

    LevelDesign::buildLevel(HEIGHT, platforms, obstacles, platformTexture, obstacleTexture, thornsTexture);
    Level2::setupSpikeRain(rain, spikeTex);

    Texture t;
    if (t.loadFromFile("Assets/Props/Leaves1.png")) propTextures.push_back(t);
    if (t.loadFromFile("Assets/Props/Tree.png"))    propTextures.push_back(t);

    LevelDesign::buildProps(WIDTH, HEIGHT, treesProp, leavesProp, propTextures);


    //Test pos frog
    //player.setPosition(13200.f, HEIGHT - 400.f);
}

bool Game::update(float dt)
{
    if (switchCooldown > 0.f) switchCooldown -= dt;

    player.updateMovement();
    player.onGround = false;

    for (auto& p : platforms) p.update(dt);
    for (auto& o : obstacles) o.update(dt, player.getPosition().x);

    rain.updateSpikeRain(dt, player.getPosition().x);
    if (rain.checkSpikeCollision(player.getGlobalBounds())) return true;

    CollisionManager::resolveAll(player, platforms, ground, player.velY, player.onGround);

    player.updateAnimation();
    syncRunSound();

    if (checkObstacleCollision()) return true;
    if (player.getPosition().y > HEIGHT + 100) return true;
    if (player.isGravityReversed && player.getPosition().y < -100) return true;

    float direction = 0;
    if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) direction = -1;
    else if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) direction = 1;

    if (direction != 0) {
        if (!(player.currentForm == Player::FROG && player.currentState == Player::JUMP)) {
            bg.update(dt, direction, 3, bg.layerCount);
        }
    }
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

            // CASE 2: FROG ITEM (Added this check!)
            if (obstacles[i].getType() == Obstacle::FROG_ITEM) {
                player.switchForm(); // Transform to Frog
                if (soundMgr) soundMgr->playSFX("jump");

                // Delete item so we don't hit it again
                obstacles.erase(obstacles.begin() + i);
                return false; // Return FALSE so we don't die!
            }

            // CASE 3: DEADLY TRAP
            if (soundMgr) soundMgr->stopSFX("run");
            return true;
        }
    }
    return false;
}

void Game::draw(RenderWindow& window) {
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

void Game::syncRunSound() {
    if (!soundMgr) return;
    if (player.isRunningOnGround()) soundMgr->playSFX("run", true);
    else soundMgr->stopSFX("run");
}

bool Game::isVisible(const sf::Sprite& sprite) {
    sf::FloatRect camRect(camera.getCenter().x - WIDTH / 2.f, camera.getCenter().y - HEIGHT / 2.f, WIDTH, HEIGHT);
    return sprite.getGlobalBounds().intersects(camRect);
}

void Game::reset() {
    player.hitbox.setPosition(300.f, 300.f);
    player.velY = 0.f;
    player.onGround = false;
    player.setGravityReversed(false);

    // Force switch back to Human on reset
    if (player.currentForm == Player::FROG) player.switchForm();

    player.currentState = Player::IDLE;
    player.currentFrame = 0;
    player.sprite.setTexture(player.tIdle);
    player.sprite.setTextureRect(IntRect(0, 0, player.frameW, player.frameH));
    camera.setCenter(WIDTH / 2.f, HEIGHT / 2.f);
    switchCooldown = 0.f;
    syncRunSound();

    platforms.clear();
    obstacles.clear();
    LevelDesign::buildLevel(HEIGHT, platforms, obstacles, platformTexture, obstacleTexture, thornsTexture);
}