#include "Ghost.h"
#include "../../MathTools.h"
#include "../Consts.h"
#include <cmath>


void Ghost::init(Vector3D& position, Room* currentRoom, GameMap* currentMap)
{
    Actor::init(currentRoom, currentMap);
    collisionBodyOffset = Vector3D(0.f, 55.f, 0.f);
    collisionBodyRadius = 16;
    animationFrame = 0;
    animationProgress = 0.f;
    animationSubset = 0;
    health = 50;

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

void Ghost::update(float deltaTime, GameMap* map, Actor* dud, ActorContainer* actors)
{

    if (isDead())
    {
        return;
    }

    Dude* dude = static_cast<Dude*>(dud);


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


    Vector3D newPos;

    if (dude->isWeaponEquiped() == Consts::flashLightId && dude->getAmmoInWeaponCount())
    {

        
        SPolygon* flashlightPoly = dude->getCurrentHelperPolygon();
        Vector3D* dudePos = dude->getPos();
        

        if (flashlightPoly)
        {
            bool collidesWithFlashlight = false;

            for (unsigned i = 1; i < flashlightPoly->points.count(); ++i)
            {

                Vector3D* p1 = &(flashlightPoly->points[i - 1]);
                Vector3D* p2 = &(flashlightPoly->points[i]);

                if (CollisionCircleLineSegment(dudePos->x + p1->x * (dude->isFlipedX() ? 1 : -1),
                                              dudePos->y + p1->y,
                                              dudePos->x + p2->x * (dude->isFlipedX() ? 1 : -1),
                                              dudePos->y + p2->y,
                                              pos.x, pos.y, 32))
                {
                    collidesWithFlashlight = true;
                    break;
                }

            }

            if (collidesWithFlashlight)
            {
                setHealth(health - 1);
            }

        }


        Vector3D dudesDirection = *(dude->getDirection());
        dudesDirection.normalize();

        float dot = direction * dudesDirection;


        if (dot < 0.f)
        {

            Vector3D newDir;
            newDir.x = dudesDirection.x * 1.5f;
            newDir.y = dudesDirection.y * 1.5f;
            newPos = pos + newDir;

            if (!Actor::isColiding(newPos, nullptr, *map))
            {
                pos = newPos;
            }

            return;
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
    newPos = pos + direction;

    if (!Actor::isColiding(newPos, nullptr, *map))
    {
        pos = newPos;
    }



}
