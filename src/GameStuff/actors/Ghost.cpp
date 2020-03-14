#include "Ghost.h"
#include "../../MathTools.h"
#include <cmath>


void Ghost::init(Vector3D& position, Room* currentRoom, GameMap* currentMap)
{
    Actor::init(currentRoom, currentMap);
    collisionBodyOffset = Vector3D(0.f, 45.f, 0.f);
    collisionBodyRadius = 20;
    animationFrame = 0;
    animationProgress = 0.f;
    animationSubset = 0;

    pos = position;


    FrameSet side;
    side.frames.add(0);
    FrameSet down;
    down.frames.add(1);
    FrameSet up;
    up.frames.add(2);
    animations.add(side);
    animations.add(down);
    animations.add(up);
    pictureIndex = 18;
}

void Ghost::update(float deltaTime, GameMap* map, Actor* dude, ActorContainer* actors)
{

    if (isDead())
    {
        return;
    }


    Vector3D dudPos = *(dude->getPos());
    Vector3D myPos = pos;
    myPos = myPos + collisionBodyOffset;
    dudPos.y += 39.f;
    Vector3D direction = dudPos - myPos;
    direction.normalize();

    animationSubset = 1;

    if (fabsf(direction.x) > fabsf(direction.y))
    {
        if (direction.x > 0.f)
        {
            flipX(true);
            animationSubset = 0;
        }
        
        if (direction.x < 0.f)
        {
            flipX(false);
            animationSubset = 0;
        }
    }
    else if (fabsf(direction.x) < fabsf(direction.y))
    {
        if (direction.y > 0.f)
        {
            animationSubset = 1;
        }
        else
        {
            animationSubset = 2;
        }
    }

    if (CollisionCircleCircle(dudPos.x, dudPos.y, 16,
                              myPos.x, myPos.y, 10))
    {
        dude->setHealth(dude->getHealth() - 1);
        return;
    }

    direction.x *= 1.2f;
    direction.y *= 1.2f;
    pos = pos + direction;



}
