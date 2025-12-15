#include "LevelDesign.h"
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace sf;

void LevelDesign::buildLevel(float height, vector<Platform>& platforms, vector<Obstacle>& obstacles, const Texture& platformTex, const Texture& obstacleTex, const Texture& thornsTex)
{
    // Ground level is at Height - 200.
    float GROUND_TOP = height - 200.f;
    float SPIKE_Y = GROUND_TOP - 80.f;
    float THORNS_Y = GROUND_TOP - 50.f;

    // --- 1. HUMAN SECTION ---


    platforms.emplace_back(platformTex, 900.f, GROUND_TOP - 150.f, 200.f, 50.f);
    platforms.emplace_back(platformTex, 1300.f, GROUND_TOP - 300.f, 200.f, 50.f);

    Platform lift(platformTex, 1700.f, GROUND_TOP - 200.f, 200.f, 50.f);
    lift.setMovement(0.f, -300.f, 2.f);
    platforms.push_back(lift);
    obstacles.emplace_back(thornsTex, 1750.f, THORNS_Y, 100.f, 50.f, Obstacle::STATIC);

    platforms.emplace_back(platformTex, 2100.f, GROUND_TOP - 450.f, 400.f, 50.f);
    Platform bridge(platformTex, 2600.f, GROUND_TOP - 450.f, 500.f, 50.f);
    platforms.push_back(bridge);
    obstacles.emplace_back(obstacleTex, 2700.f, GROUND_TOP - 850.f, 80.f, -80.f, Obstacle::FALLING);
    obstacles.emplace_back(obstacleTex, 2900.f, GROUND_TOP - 850.f, 80.f, -80.f, Obstacle::FALLING);

    // Floor traps
    obstacles.emplace_back(obstacleTex, 2700.f, THORNS_Y, 100.f, 50.f, Obstacle::STATIC);
    obstacles.emplace_back(obstacleTex, 2900.f, THORNS_Y, 100.f, 50.f, Obstacle::STATIC);

    Platform mover1(platformTex, 3300.f, GROUND_TOP - 400.f, 150.f, 40.f);
    mover1.setMovement(300.f, 0.f, 1.5f);
    platforms.push_back(mover1);
    obstacles.emplace_back(thornsTex, 3300.f, THORNS_Y, 400.f, 50.f, Obstacle::STATIC);

    Platform mover2(platformTex, 3900.f, GROUND_TOP - 400.f, 150.f, 40.f);
    mover2.setMovement(-300.f, 0.f, 1.5f);
    platforms.push_back(mover2);
    obstacles.emplace_back(thornsTex, 3900.f, THORNS_Y, 400.f, 50.f, Obstacle::STATIC);

    platforms.emplace_back(platformTex, 4500.f, GROUND_TOP - 300.f, 800.f, 50.f);
    obstacles.emplace_back(thornsTex, 4700.f, GROUND_TOP - 380.f, 90.f, 90.f, Obstacle::ROTATING);
    obstacles.emplace_back(thornsTex, 4900.f, GROUND_TOP - 380.f, 90.f, 90.f, Obstacle::ROTATING);
    obstacles.emplace_back(thornsTex, 5100.f, GROUND_TOP - 380.f, 90.f, 90.f, Obstacle::ROTATING);

    platforms.emplace_back(platformTex, 5500.f, GROUND_TOP - 200.f, 200.f, 50.f);
    Platform hard(platformTex, 5900.f, GROUND_TOP - 200.f, 100.f, 40.f);
    hard.setMovement(0.f, 200.f, 4.f);
    platforms.push_back(hard);
    obstacles.emplace_back(thornsTex, 5900.f, THORNS_Y, 150.f, 50.f, Obstacle::STATIC);

    // --- GRAVITY SWITCH ---
    platforms.emplace_back(platformTex, 6800.f, GROUND_TOP - 100.f, 200.f, 50.f);
    obstacles.emplace_back(obstacleTex, 6900.f, GROUND_TOP - 180.f, 50.f, 50.f, Obstacle::GRAVITY_SWITCH);

    platforms.emplace_back(platformTex, 7100.f, 100.f, 600.f, 50.f); // Ceiling
    obstacles.emplace_back(obstacleTex, 7300.f, 150.f, 100.f, 100.f, Obstacle::STATIC);

    Platform ceilMover(platformTex, 7800.f, 100.f, 200.f, 50.f);
    ceilMover.setMovement(0.f, 200.f, 2.f);
    platforms.push_back(ceilMover);

    platforms.emplace_back(platformTex, 8200.f, 100.f, 800.f, 50.f);
    obstacles.emplace_back(thornsTex, 8400.f, 150.f, 90.f, 90.f, Obstacle::ROTATING);
    obstacles.emplace_back(thornsTex, 8700.f, 150.f, 90.f, 90.f, Obstacle::ROTATING);

    Platform ceilMover2(platformTex, 9200.f, 200.f, 150.f, 50.f);
    ceilMover2.setMovement(0.f, -150.f, 1.5f);
    platforms.push_back(ceilMover2);
    obstacles.emplace_back(obstacleTex, 9200.f, 50.f, 100.f, 100.f, Obstacle::STATIC);

    // Switch Back
    platforms.emplace_back(platformTex, 9600.f, 100.f, 200.f, 50.f);
    obstacles.emplace_back(obstacleTex, 9800.f, 150.f, 50.f, 50.f, Obstacle::GRAVITY_SWITCH);
    obstacles.emplace_back(obstacleTex, 9800.f, 250.f, 50.f, 50.f, Obstacle::GRAVITY_SWITCH);
    obstacles.emplace_back(obstacleTex, 9800.f, 350.f, 50.f, 50.f, Obstacle::GRAVITY_SWITCH);

    platforms.emplace_back(platformTex, 9900.f, GROUND_TOP - 200.f, 400.f, 50.f);

    // Falling Platforms
    Platform fall1(platformTex, 10400.f, GROUND_TOP - 200.f, 120.f, 40.f);
    platforms.push_back(fall1);
    obstacles.emplace_back(obstacleTex, 10400.f, GROUND_TOP - 600.f, 100.f, 100.f, Obstacle::FALLING);
    Platform fall2(platformTex, 10700.f, GROUND_TOP - 100.f, 120.f, 40.f);
    platforms.push_back(fall2);
    Platform fall3(platformTex, 11000.f, GROUND_TOP - 300.f, 120.f, 40.f);
    platforms.push_back(fall3);
    for (float x = 10300.f; x < 11200.f; x += 100.f) {
        obstacles.emplace_back(thornsTex, x, THORNS_Y, 100.f, 50.f, Obstacle::STATIC);
    }

    // Tunnel
    float TUNNEL_Y = GROUND_TOP - 100.f;
    platforms.emplace_back(platformTex, 11300.f, TUNNEL_Y, 800.f, 50.f);
    obstacles.emplace_back(thornsTex, 11500.f, THORNS_Y, 100.f, 50.f, Obstacle::STATIC);
    obstacles.emplace_back(obstacleTex, 11700.f, TUNNEL_Y - 400.f, 80.f, 80.f, Obstacle::FALLING);

    // Boss Jump
    Platform bossPlat(platformTex, 12300.f, GROUND_TOP - 200.f, 150.f, 40.f);
    bossPlat.setMovement(0.f, 300.f, 3.f);
    platforms.push_back(bossPlat);
    obstacles.emplace_back(thornsTex, 12300.f, height - 600.f, 90.f, 90.f, Obstacle::ROTATING);

    platforms.emplace_back(platformTex, 12600.f, GROUND_TOP - 100.f, 400.f, 50.f);


    // --- 8. FROG PHASE (REDESIGNED) ---
 

    platforms.emplace_back(platformTex, 13200.f, GROUND_TOP - 100.f, 400.f, 50.f);
    obstacles.emplace_back(obstacleTex, 13400.f, GROUND_TOP - 180.f, 50.f, 50.f, Obstacle::FROG_ITEM);


    for (float tx = 13600.f; tx < 18000.f; tx += 50.f) {
        obstacles.emplace_back(thornsTex, tx, THORNS_Y, 50.f, 50.f, Obstacle::STATIC);
    }

    float frogX = 13800.f;

    platforms.emplace_back(platformTex, frogX, GROUND_TOP - 300.f, 200.f, 50.f);

    platforms.emplace_back(platformTex, frogX + 400.f, GROUND_TOP - 550.f, 200.f, 50.f);

    Platform skyMover1(platformTex, frogX + 800.f, GROUND_TOP - 550.f, 150.f, 40.f);
    skyMover1.setMovement(200.f, 0.f, 2.f); // Horizontal move
    platforms.push_back(skyMover1);


    float rainStartX = frogX + 1300.f; // ~15100

    platforms.emplace_back(platformTex, rainStartX, GROUND_TOP - 400.f, 300.f, 50.f);

    platforms.emplace_back(platformTex, rainStartX + 400.f, GROUND_TOP - 200.f, 300.f, 50.f);

    platforms.emplace_back(platformTex, rainStartX + 800.f, GROUND_TOP - 500.f, 300.f, 50.f);

    obstacles.emplace_back(thornsTex, rainStartX + 500.f, GROUND_TOP - 800.f, 90.f, 90.f, Obstacle::ROTATING);



    float towerX = rainStartX + 1300.f; // ~16400
    float towerY = GROUND_TOP - 200.f;

    platforms.emplace_back(platformTex, towerX, towerY, 200.f, 50.f); // Base

    // Zig Zag Up (280px jumps for -27.f jump force)
    towerY -= 280.f;
    platforms.emplace_back(platformTex, towerX + 150.f, towerY, 200.f, 50.f);

    towerY -= 280.f;
    platforms.emplace_back(platformTex, towerX - 150.f, towerY, 200.f, 50.f);

    towerY -= 300.f;
    Platform towerElevator(platformTex, towerX, towerY, 150.f, 40.f);
    towerElevator.setMovement(0.f, 200.f, 2.f); // Vertical move
    platforms.push_back(towerElevator);



    platforms.emplace_back(platformTex, towerX + 600.f, GROUND_TOP - 200.f, 1000.f, 50.f);
}

void LevelDesign::buildProps(float width, float height, vector<Sprite>& trees, vector<Sprite>& leaves, const vector<Texture>& propTextures)
{
    srand((unsigned)(time(0)));
    int numProps = 600;

    for (int i = 0; i < numProps; i++) {
        Sprite s;
        int typeIndex = rand() % propTextures.size();
        s.setTexture(propTextures[typeIndex]);

        // Extended range for long level
        float x = 100.f + (float)(rand()) / RAND_MAX * (width * 18.f - 200.f);
        float y = 0;
        float GROUND_TOP = height - 200.f;

        if (typeIndex == 0) {
            y = GROUND_TOP - s.getTexture()->getSize().y * 0.4f;
            s.setScale(0.4f, 0.4f);
        }
        else {
            y = height - 50 - s.getTexture()->getSize().y;
            s.setScale(1.f, 1.f);
        }
        s.setPosition(x, y);
        if (typeIndex == 0) leaves.push_back(s);
        else trees.push_back(s);
    }
}