#include "Rat.h"
#include "../Usefull.h"

void Rat::init(Vector3D& position)
{
    pos = position;
    animationSubset = 1;
    animationProgress = 0.f;
    animationFrame = 0.f;
    collisionBodyRadius = 10;
    collisionBodyOffset = Vector3D(0, 0, 0);

    FrameSet up;
    up.frames.add(0);
    up.frames.add(1);
    up.frames.add(2);
    up.frames.add(1);
    FrameSet down;
    down.frames.add(5);
    down.frames.add(6);
    down.frames.add(7);
    down.frames.add(6);
    FrameSet side;
    side.frames.add(10);
    side.frames.add(11);
    side.frames.add(12);
    side.frames.add(11);
    FrameSet attack;
    attack.frames.add(11);
    attack.frames.add(13);
    attack.frames.add(14);
    attack.frames.add(11);


    animations.add(up);
    animations.add(down);
    animations.add(side);
    animations.add(attack);
    strcpy(spriteName, "pics/rat.tga");
}

void Rat::update(float deltaTime, GameMap& map, Dude& dude, ActorContainer& actors)
{

    Vector3D dudPos = *dude.getPos();
    dudPos.y += 39.f;
    Vector3D direction = dudPos - pos;
    direction.normalize();

    animationSubset = 0;

    if (fabsf(direction.x) > fabsf(direction.y))
    {
        if (direction.x > 0.f)
        {
            isFlipedX = true;
            animationSubset = 2;
        }
        
        if (direction.x < 0.f)
        {
            isFlipedX = false;
            animationSubset = 2;
        }
    }
    else if (fabsf(direction.x) < fabsf(direction.y))
    {
        if (direction.y > 0.f)
        {
            animationSubset = 0;
        }
        else
        {
            animationSubset = 1;
        }
    }

    Vector3D newPos = pos + direction;

    if (!Actor::isColiding(newPos, map) && !actors.isColidingWithOthers(this, direction))
    {
        pos = newPos;
    }
    

    if (CollisionCircleCircle(pos.x, pos.y, 8, dudPos.x, dudPos.y, 38))
    {
        animationSubset = 3;
    }

    animationProgress += deltaTime * 6.f;

    if (animationProgress >= 1.f)
    {
        ++animationFrame;
        animationProgress = 0.f;

        if (animationFrame > 3)
        {
            if (animationSubset == 3)
            {
                dude.setHealth(dude.getHealth() - 1);
            }
            animationFrame = 0;
        }
    }

}
