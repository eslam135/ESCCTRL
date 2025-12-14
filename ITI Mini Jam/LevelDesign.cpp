#include "LevelDesign.h"
#include <iostream>

LevelDesign::LevelDesign(float gameWidth, float gameHeight)
    : WIDTH(gameWidth), HEIGHT(gameHeight)
{
}

void LevelDesign::loadLevel(std::vector<Platform>& platforms, std::vector<Obstacle>& obstacles, sf::Texture& platformTexture, sf::Texture& obstacleTexture)
{
    // Load textures
    if (!platformTexture.loadFromFile("Assets/Platforms/Pltfrm1.png"))
        std::cerr << "Couldn't load platform texture\n";
    if (!obstacleTexture.loadFromFile("Assets/Spikes/Spikes2.png"))
        std::cerr << "Couldn't load obstacle texture\n";

    const float platformH = 80.f;
    const float trapW = 100.f;
    const float trapH = 85.f;

    // Platforms
    platforms.emplace_back(platformTexture, 500.f, HEIGHT - 300.f, 300.f, platformH);
    platforms.emplace_back(platformTexture, 950.f, HEIGHT - 380.f, 250.f, platformH);
    platforms.emplace_back(platformTexture, 1400.f, HEIGHT - 320.f, 400.f, platformH);
    platforms.emplace_back(platformTexture, 2000.f, HEIGHT - 420.f, 200.f, platformH);
    platforms.emplace_back(platformTexture, 2400.f, HEIGHT - 360.f, 300.f, platformH);
   
    platforms.emplace_back(platformTexture, 250.f, HEIGHT - 450.f, 200.f, 60.f);

	// ---------Moving platform (horizontal) trial--------- //
    platforms.back().setMoving(Platform::MoveAxis::Horizontal,
                              /*left*/ 250.f,   /*right*/ 1100.f,
                             /*speed*/ 150.f,
                            /*trigger*/ true, 
                            Platform::MoveMode::OneWay);


    platforms.emplace_back(platformTexture, 1100.f, HEIGHT - 450.f, 200.f, 60.f);

	// ---------Moving platform (vertical) trial--------- //
    platforms.back().setMoving(Platform::MoveAxis::Vertical,
                              /*top*/ HEIGHT - 800.f,   /*bottom*/ HEIGHT - 400.f,
                             /*speed*/ 100.f,
                            /*trigger*/ true,
                            Platform::MoveMode::PingPong);


    // Traps
    obstacles.emplace_back(obstacleTexture, 800.f, HEIGHT - 200.f - trapH, 120.f, trapH);
    obstacles.emplace_back(obstacleTexture, 1300.f, HEIGHT - 200.f - trapH, 150.f, trapH);
    obstacles.emplace_back(obstacleTexture, 1900.f, HEIGHT - 200.f - trapH, 200.f, trapH);
    obstacles.emplace_back(obstacleTexture, 2300.f, HEIGHT - 200.f - trapH, 120.f, trapH);
    obstacles.emplace_back(obstacleTexture, 3000.f, HEIGHT - 200.f - trapH, 150.f, trapH);
    obstacles.emplace_back(obstacleTexture, 3150.f, HEIGHT - 200.f - trapH, 150.f, trapH);
    obstacles.emplace_back(obstacleTexture, 3300.f, HEIGHT - 200.f - trapH, 150.f, trapH);
    obstacles.emplace_back(obstacleTexture, 3450.f, HEIGHT - 200.f - trapH, 150.f, trapH);
    obstacles.emplace_back(obstacleTexture, 3600.f, HEIGHT - 200.f - trapH, 150.f, trapH);
    obstacles.emplace_back(obstacleTexture, 3750.f, HEIGHT - 200.f - trapH, 150.f, trapH);
    obstacles.emplace_back(obstacleTexture, 3900.f, HEIGHT - 200.f - trapH, 150.f, trapH);
}
