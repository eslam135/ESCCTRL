#include "../include/Game.h"
#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include "../include/LevelDesign.h"
#include "../include/Level2.h"

using namespace sf;
using namespace std;

const float GAME_WIDTH = 1920.f;
const float GAME_HEIGHT = 1080.f;

Game::Game(float W, float H, SoundManager* sm)
    : WIDTH(GAME_WIDTH), HEIGHT(GAME_HEIGHT),
    rain(80, W, H),
    bg(5, GAME_WIDTH * 10000.f, GAME_HEIGHT, { 0.f, 0.f , 12.f, 18.f , 60.f }, 0),
    BGground(1, GAME_WIDTH * 10000.f, GAME_HEIGHT, { 0.f }, 5),
    ground(0.f, GAME_HEIGHT - 200.f, GAME_WIDTH * 10000.f - 100.f, 200.f, Color(0, 255, 0, 0))
{
    soundMgr = sm;
    player.soundMgr = sm;
    WORLD_RIGHT = GAME_WIDTH * 10000.f;

    camera.setSize(GAME_WIDTH, GAME_HEIGHT);
    camera.setCenter(GAME_WIDTH / 2.f, GAME_HEIGHT / 2.f);

    // --- PREVENT DEFORMATION (Letterboxing) ---
    // This math calculates if we need black bars on top/bottom or sides
    // so the game is never stretched or squashed.
    float screenRatio = W / H;
    float gameRatio = GAME_WIDTH / GAME_HEIGHT;

    if (screenRatio > gameRatio)
    {
        // Screen is wider than game -> Bars on sides
        float scale = H / GAME_HEIGHT;
        float widthOffset = (W - GAME_WIDTH * scale) / 2.f;
        camera.setViewport(sf::FloatRect(widthOffset / W, 0.f, (GAME_WIDTH * scale) / W, 1.f));
    }
    else
    {
        // Screen is taller than game -> Bars on top/bottom
        float scale = W / GAME_WIDTH;
        float heightOffset = (H - GAME_HEIGHT * scale) / 2.f;
        camera.setViewport(sf::FloatRect(0.f, heightOffset / H, 1.f, (GAME_HEIGHT * scale) / H));
    }

    if (!platformTexture.loadFromFile("Assets/Platforms/Pltfrm1.png")) cerr << "Platform tex missing\n";
    if (!obstacleTexture.loadFromFile("Assets/Spikes/Spikes2.png")) cerr << "Spike tex missing\n";
    if (!circleThornsTexture.loadFromFile("Assets/Thorns/circleThorns.png")) cerr << "Circle thorns tex missing\n";
    if (!shortThornsTexture.loadFromFile("Assets/Thorns/shortThorns.png")) cerr << "Short thorns tex missing\n";
    if (!mediumThornsTexture.loadFromFile("Assets/Thorns/mediumThorns.png")) cerr << "Medium thorns tex missing\n";
    if (!longThornsTexture.loadFromFile("Assets/Thorns/longThorns.png")) cerr << "Long thorns tex missing\n";
    if (!spikeTex.loadFromFile("Assets/Spikes/Spike.png")) cerr << "Spike rain tex missing\n";

    LevelDesign::buildLevel(GAME_HEIGHT, platforms, obstacles,
        platformTexture, obstacleTexture, circleThornsTexture, shortThornsTexture, mediumThornsTexture, longThornsTexture);

    Level2::setupSpikeRain(rain, spikeTex);

    // ---------- WIN MARKER ----------
    if (!winMarkerTex.loadFromFile("Assets/villageSign.png"))
        cerr << "WinMarker missing\n";

    winMarker.setTexture(winMarkerTex);
    winMarker.setScale(0.3f, 0.3f);
    winMarker.setPosition(WIN_X, GAME_HEIGHT - 410.f);

    Texture t;
    if (t.loadFromFile("Assets/Props/Leaves1.png")) propTextures.push_back(t);
    if (t.loadFromFile("Assets/Props/Tree.png"))    propTextures.push_back(t);
    if (t.loadFromFile("Assets/Props/Leaves2.png")) propTextures.push_back(t);
    if (t.loadFromFile("Assets/Props/Leaves3.png")) propTextures.push_back(t);

    LevelDesign::buildProps(GAME_WIDTH, GAME_HEIGHT, treesProp, leavesProp, propTextures);


    
    //player.setPosition(9950.f, HEIGHT - 400.f); // gravity switch part
    //player.setPosition(13200.f, HEIGHT - 400.f); // frog shapeshift part
    //player.setPosition(17900.f, HEIGHT - 400.f); // win sign part

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

    // Check bounds using fixed height
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

    // Keep camera height centered at fixed height
    camera.setCenter(px, HEIGHT / 2.f);

    return false;
}

bool Game::hasWon() const
{
    return player.getPosition().x >= WIN_X;
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

            // CASE 2: FROG ITEM
            if (obstacles[i].getType() == Obstacle::FROG_ITEM) {
                player.switchForm(); // Transform to Frog
                if (soundMgr) soundMgr->playSFX("jump");
                obstacles.erase(obstacles.begin() + i);
                return false;
            }

            // CASE 3: DEADLY TRAP
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
    for (auto& o : obstacles) o.draw(window);

    for (auto& plat : platforms) plat.draw(window);


    window.draw(winMarker);

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

    if (player.currentForm == Player::FROG) player.switchForm();

    player.currentState = Player::IDLE;
    player.currentFrame = 0;
    player.sprite.setTexture(player.tIdle);
    player.sprite.setTextureRect(IntRect(0, 0, player.frameW, player.frameH));

    // Reset camera to fixed center
    camera.setCenter(WIDTH / 2.f, HEIGHT / 2.f);
    switchCooldown = 0.f;
    syncRunSound();

    platforms.clear();
    obstacles.clear();

    LevelDesign::buildLevel(HEIGHT, platforms, obstacles, platformTexture, obstacleTexture, circleThornsTexture, shortThornsTexture,mediumThornsTexture,longThornsTexture);
}