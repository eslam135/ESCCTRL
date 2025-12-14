#include "LevelDesign.h"
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace sf;

void LevelDesign::buildLevel(float height, vector<Platform>& platforms, vector<Obstacle>& obstacles, const Texture& platformTex, const Texture& obstacleTex, const Texture& thornsTex)
{
    // --- 1. THE STANDARD LEVEL ---
    platforms.emplace_back(platformTex, 200.f, height - 200.f, 600.f, 80.f);
    platforms.emplace_back(platformTex, 900.f, height - 300.f, 200.f, 50.f);
    platforms.emplace_back(platformTex, 1300.f, height - 400.f, 200.f, 50.f);

    Platform lift(platformTex, 1700.f, height - 250.f, 200.f, 50.f);
    lift.setMovement(0.f, -300.f, 2.f);
    platforms.push_back(lift);

    obstacles.emplace_back(obstacleTex, 1750.f, height - 350.f, 100.f, 100.f, Obstacle::STATIC);

    platforms.emplace_back(platformTex, 2100.f, height - 550.f, 400.f, 50.f);

    Platform bridge(platformTex, 2600.f, height - 550.f, 500.f, 50.f);
    platforms.push_back(bridge);
    obstacles.emplace_back(obstacleTex, 2700.f, height - 900.f, 80.f, -80.f, Obstacle::FALLING);
    obstacles.emplace_back(obstacleTex, 2900.f, height - 900.f, 80.f, -80.f, Obstacle::FALLING);
    obstacles.emplace_back(obstacleTex, 2600.f, height - 350.f, 100.f, 100.f, Obstacle::STATIC);

    Platform mover1(platformTex, 3300.f, height - 500.f, 150.f, 40.f);
    mover1.setMovement(300.f, 0.f, 1.5f);
    platforms.push_back(mover1);
    obstacles.emplace_back(obstacleTex, 3300.f, height - 350.f, 400.f, 100.f, Obstacle::STATIC);

    Platform mover2(platformTex, 3900.f, height - 500.f, 150.f, 40.f);
    mover2.setMovement(-300.f, 0.f, 1.5f);
    platforms.push_back(mover2);
    obstacles.emplace_back(obstacleTex, 3900.f, height - 350.f, 400.f, 100.f, Obstacle::STATIC);

    platforms.emplace_back(platformTex, 4200.f, height - 450.f, 200.f, 50.f);

    platforms.emplace_back(platformTex, 4500.f, height - 400.f, 800.f, 50.f);
    obstacles.emplace_back(thornsTex, 4700.f, height - 480.f, 90.f, 90.f, Obstacle::ROTATING);
    obstacles.emplace_back(thornsTex, 4900.f, height - 480.f, 90.f, 90.f, Obstacle::ROTATING);
    obstacles.emplace_back(thornsTex, 5100.f, height - 480.f, 90.f, 90.f, Obstacle::ROTATING);

    platforms.emplace_back(platformTex, 5500.f, height - 300.f, 200.f, 50.f);

    Platform hard(platformTex, 5900.f, height - 300.f, 100.f, 40.f);
    hard.setMovement(0.f, 200.f, 4.f);
    platforms.push_back(hard);
    obstacles.emplace_back(obstacleTex, 5900.f, height - 350.f, 150.f, 100.f, Obstacle::STATIC);

    platforms.emplace_back(platformTex, 6300.f, height - 200.f, 500.f, 50.f);


    // --- 2. GRAVITY SWITCH SECTION ---

    platforms.emplace_back(platformTex, 6800.f, height - 200.f, 200.f, 50.f);
    obstacles.emplace_back(obstacleTex, 6900.f, height - 280.f, 50.f, 50.f, Obstacle::GRAVITY_SWITCH);

    platforms.emplace_back(platformTex, 7100.f, 100.f, 600.f, 50.f);
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


    // --- 3. SWITCH BACK TO NORMAL ---

    platforms.emplace_back(platformTex, 9600.f, 100.f, 200.f, 50.f);

    //3 Switches to gurantee player hit them and switch gravity back to normal
    obstacles.emplace_back(obstacleTex, 9800.f, 150.f, 50.f, 50.f, Obstacle::GRAVITY_SWITCH);
    obstacles.emplace_back(obstacleTex, 9800.f, 250.f, 50.f, 50.f, Obstacle::GRAVITY_SWITCH);
    obstacles.emplace_back(obstacleTex, 9800.f, 350.f, 50.f, 50.f, Obstacle::GRAVITY_SWITCH);

    // Landing Zone (Back on Floor)
    // Moved slightly further to give space for the flip
    platforms.emplace_back(platformTex, 9900.f, height - 400.f, 400.f, 50.f);


    Platform fall1(platformTex, 10400.f, height - 400.f, 120.f, 40.f);
    platforms.push_back(fall1);
    obstacles.emplace_back(obstacleTex, 10400.f, height - 700.f, 100.f, -100.f, Obstacle::FALLING);

    Platform fall2(platformTex, 10700.f, height - 300.f, 120.f, 40.f);
    platforms.push_back(fall2);

    Platform fall3(platformTex, 11000.f, height - 500.f, 120.f, 40.f);
    platforms.push_back(fall3);

    for (float x = 10300.f; x < 11200.f; x += 100.f) {
        obstacles.emplace_back(obstacleTex, x, height - 350.f, 100.f, 100.f, Obstacle::STATIC);
    }

    float TUNNEL_Y = height - 300.f;
    platforms.emplace_back(platformTex, 11300.f, TUNNEL_Y, 800.f, 50.f);
    obstacles.emplace_back(obstacleTex, 11500.f, height - 450.f, 100.f, 100.f, Obstacle::STATIC);
    obstacles.emplace_back(obstacleTex, 11700.f, TUNNEL_Y - 600.f, 80.f, -80.f, Obstacle::FALLING);

    Platform bossPlat(platformTex, 12300.f, height - 400.f, 150.f, 40.f);
    bossPlat.setMovement(0.f, 300.f, 3.f);
    platforms.push_back(bossPlat);
    obstacles.emplace_back(thornsTex, 12300.f, height - 600.f, 90.f, 90.f, Obstacle::ROTATING);

    platforms.emplace_back(platformTex, 12600.f, height - 400.f, 1000.f, 50.f);
}

void LevelDesign::buildProps(float width, float height, vector<Sprite>& trees, vector<Sprite>& leaves, const vector<Texture>& propTextures)
{
    srand((unsigned)(time(0)));
    int numProps = 400;

    for (int i = 0; i < numProps; i++) {
        Sprite s;
        int typeIndex = rand() % propTextures.size();
        s.setTexture(propTextures[typeIndex]);

        float x = 100.f + (float)(rand()) / RAND_MAX * (width * 15.f - 200.f);
        float y = 0;
        if (typeIndex == 0) {
            float groundTop = height - 200;
            y = groundTop - s.getTexture()->getSize().y * 0.4f;
            s.setScale(0.4f, 0.4f);
        }
        else {
            float groundTop = height - 50;
            y = groundTop - s.getTexture()->getSize().y;
            s.setScale(1.f, 1.f);
        }
        s.setPosition(x, y);

        if (typeIndex == 0) leaves.push_back(s);
        else trees.push_back(s);
    }
}