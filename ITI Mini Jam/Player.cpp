#include "Player.h"
#include <iostream>

using namespace sf;
using namespace std;

Player::Player(SoundManager* manager)
{
    soundMgr = manager;

    tIdle.loadFromFile("Assets/Character/idle.png");
    tRun.loadFromFile("Assets/Character/run.png");
    tJump.loadFromFile("Assets/Character/jump.png");

    tFrogIdle.loadFromFile("Assets/Frog/Idle.png");
    tFrogJump.loadFromFile("Assets/Frog/Jump.png");

    sprite.setTexture(tIdle);
    sprite.setTextureRect(IntRect(0, 0, frameW, frameH));
    sprite.setScale(spriteScale, spriteScale);
    sprite.setOrigin(frameW / 2.f, frameH / 2.f);

    hitbox.setSize({ 40.f, 150.f });
    hitbox.setOrigin(20.f, 75.f);
    hitbox.setPosition(300.f, 300.f);
    hitbox.setFillColor(Color::Transparent);

    maxFrames = framesIdle;
}

void Player::switchForm()
{
    if (currentForm == HUMAN)
    {
        currentForm = FROG;
        sprite.setTexture(tFrogJump);
        maxFrames = framesFrogJump;
        spriteScale = 0.1f;
        currentFrame = 0;
    }
    else
    {
        currentForm = HUMAN;
        sprite.setTexture(tIdle);
        maxFrames = framesIdle;
		spriteScale = 0.2f;
        currentFrame = 0;
    }
}

void Player::setGravityReversed(bool reversed)
{
    isGravityReversed = reversed;
}

void Player::updateMovement()
{
    float gravityDir = isGravityReversed ? -1.f : 1.f;
    movingHorizontal = false;


    if (currentForm == FROG)
    {
        float gravityDir = isGravityReversed ? -1.f : 1.f;

        if (onGround && (Keyboard::isKeyPressed(Keyboard::Space) ||
            Keyboard::isKeyPressed(Keyboard::W) ||
            Keyboard::isKeyPressed(Keyboard::Up)))
        {
            velY = -22.f * gravityDir;
            onGround = false;
            facingRight = sprite.getScale().x > 0 ? true : false;
        }

        velY += gravity * gravityDir;
        hitbox.move(0, velY);

        if (!onGround)
        {
            if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
            {
                hitbox.move(-5.f, 0);
                facingRight = false;
            }
            else if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
            {
                hitbox.move(5.f, 0);
                facingRight = true;
            }

            currentState = JUMP;
            sprite.setTexture(tFrogJump);
            maxFrames = framesFrogJump;
        }
        else
        {
            currentState = IDLE;
            sprite.setTexture(tFrogIdle);
            maxFrames = framesFrogIdle;
        }

        return; 
    }

    bool moving = false;

    if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
    {
        hitbox.move(-speed, 0);
        facingRight = false;
        moving = true;
        movingHorizontal = true;
    }
    else if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
    {
        hitbox.move(speed, 0);
        facingRight = true;
        moving = true;
        movingHorizontal = true;
    }

    if (Keyboard::isKeyPressed(Keyboard::Space) && onGround)
    {
        velY = -20.f * gravityDir;
        onGround = false;
        if (soundMgr) soundMgr->playSFX("jump");
    }

    velY += gravity * gravityDir;
    hitbox.move(0, velY);

    if (!onGround) currentState = JUMP;
    else if (moving) currentState = RUN;
    else currentState = IDLE;

    if (currentState == IDLE) { sprite.setTexture(tIdle); maxFrames = framesIdle; }
    if (currentState == RUN) { sprite.setTexture(tRun);  maxFrames = framesRun; }
    if (currentState == JUMP) { sprite.setTexture(tJump); maxFrames = framesJump; }
}

void Player::updateAnimation()
{
    timeSince += animClock.restart().asSeconds();

    animSpeed = (currentForm == FROG) ? 0.25f : 0.09f;

    if (timeSince >= animSpeed)
    {
        timeSince = 0.f;
        currentFrame = (currentFrame + 1) % maxFrames;
    }

    sprite.setTextureRect(IntRect(currentFrame * frameW, 0, frameW, frameH));

    float scaleX = facingRight ? spriteScale : -spriteScale;
    float scaleY = isGravityReversed ? -spriteScale : spriteScale;

    sprite.setScale(scaleX, scaleY);
    sprite.setPosition(hitbox.getPosition());
}

void Player::draw(RenderWindow& window)
{
    window.draw(sprite);
}

FloatRect Player::getGlobalBounds() const { return hitbox.getGlobalBounds(); }
Vector2f Player::getPosition() const { return hitbox.getPosition(); }
void Player::move(float dx, float dy) { hitbox.move(dx, dy); }
void Player::setPosition(float x, float y) { hitbox.setPosition(x, y); sprite.setPosition(x, y); }

bool Player::isRunningOnGround() const { return movingHorizontal && onGround; }
bool Player::isMovingHorizontally() const { return movingHorizontal; }

bool Player::isStandingOn(const Platform& platform) const
{
    FloatRect p = hitbox.getGlobalBounds();
    FloatRect b = platform.getBounds();
    const float tol = 10.f;

    bool vertical = !isGravityReversed
        ? (p.top + p.height <= b.top + tol && p.top + p.height >= b.top - tol)
        : (p.top >= b.top + b.height - tol && p.top <= b.top + b.height + tol);

    return vertical && (p.left + p.width > b.left && p.left < b.left + b.width);
}
