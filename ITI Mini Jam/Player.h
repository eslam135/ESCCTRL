#pragma once

#include <SFML/Graphics.hpp>
#include "SoundManager.h"
#include "platform.h"

class Player
{
public:

    enum State { IDLE, RUN, JUMP };
    enum Form { HUMAN, FROG };


    sf::Texture tIdle, tRun, tJump;         
    sf::Texture tFrogIdle, tFrogJump;        

    sf::Sprite sprite;
    sf::RectangleShape hitbox;

    int frameW = 1024, frameH = 1024;
    int framesIdle = 3, framesRun = 6, framesJump = 7;
    int framesFrogIdle = 5, framesFrogJump = 5;

    int currentState = IDLE;
    int currentFrame = 0;
    int maxFrames = 3;

    float timeSince = 0.f;
    float animSpeed = 0.1f;
    sf::Clock animClock;

    float speed = 5.f;
    float gravity = 0.6f;
    float velY = 0.f;
    bool onGround = false;
    bool facingRight = true;
    bool movingHorizontal = false;
    bool isGravityReversed = false;
    bool hopLocked = false;

    Form currentForm = HUMAN;
    bool switchPressed = false;

    float spriteScale = 0.2f;
    SoundManager* soundMgr = nullptr;

    Player(SoundManager* manager = nullptr);

    void updateMovement();
    void updateAnimation();
    void draw(sf::RenderWindow& window);


    void switchForm();

    void setGravityReversed(bool reversed);
    sf::FloatRect getGlobalBounds() const;
    sf::Vector2f getPosition() const;
    void move(float dx, float dy);
    void setPosition(float x, float y);

    bool isRunningOnGround() const;
    bool isMovingHorizontally() const;
    bool isStandingOn(const Platform& platform) const;
};
