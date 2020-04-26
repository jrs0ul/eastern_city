#include "Bear.h"
#include "../../MathTools.h"
#include "../ActorContainer.h"
#include "../map/GameMap.h"

void Bear::init(Vector3D& position, Room* currentRoom, GameMap* currentMap)
{
    Actor::init(currentRoom, currentMap);
    pos = position;
    pictureIndex = 32;
    animationSubset = 1;
    animationFrame = 0.f;
    collisionBodyRadius = 32;
    collisionBodyOffset = Vector3D(0, 28, 0);
    animationProgress = 0.f;
    health = 120;

    loot.add(24);
    loot.add(24);
    loot.add(24);
    loot.add(24);
    loot.add(4);
    loot.add(4);
    loot.add(4);

    FrameSet walkUp;
    walkUp.frames.add(5);
    walkUp.frames.add(6);
    walkUp.frames.add(7);
    walkUp.frames.add(6);
    walkUp.targetingBox.pos = Vector3D(-40, -50, 0);
    walkUp.targetingBox.size = Vector3D(80, 100, 0);

    FrameSet walkDown;
    walkDown.frames.add(0);
    walkDown.frames.add(1);
    walkDown.frames.add(2);
    walkDown.frames.add(1);
    walkDown.targetingBox.pos = Vector3D(-40, -50, 0);
    walkDown.targetingBox.size = Vector3D(80, 100, 0);

    FrameSet walkSide;
    walkSide.frames.add(10);
    walkSide.frames.add(11);
    walkSide.frames.add(12);
    walkSide.frames.add(11);
    walkSide.targetingBox.pos = Vector3D(-90, -50, 0);
    walkSide.targetingBox.size = Vector3D(170, 100, 0);

    FrameSet attackUp;
    attackUp.frames.add(6);
    attackUp.frames.add(8);
    attackUp.frames.add(9);
    attackUp.frames.add(6);
    attackUp.targetingBox.pos = Vector3D(-40, -50, 0);
    attackUp.targetingBox.size = Vector3D(80, 100, 0);

    FrameSet attackDown;
    attackDown.frames.add(1);
    attackDown.frames.add(3);
    attackDown.frames.add(4);
    attackDown.frames.add(1);
    attackDown.targetingBox.pos = Vector3D(-40, -50, 0);
    attackDown.targetingBox.size = Vector3D(80, 100, 0);

    FrameSet attackSide;
    attackSide.frames.add(11);
    attackSide.frames.add(13);
    attackSide.frames.add(14);
    attackSide.frames.add(11);
    attackSide.targetingBox.pos = Vector3D(-90, -50, 0);
    attackSide.targetingBox.size = Vector3D(170, 100, 0);


    animations.add(walkUp);
    animations.add(walkDown);
    animations.add(walkSide);
    animations.add(attackUp);
    animations.add(attackDown);
    animations.add(attackSide);
}

void Bear::update(float deltaTime, GameMap* map, Actor* dude, ActorContainer* actors)
{
    if (isDead())
    {
        return;
    }

    updateDamage(deltaTime);

    Vector3D dudPos = *(dude->getPos());
    dudPos.y += 39.f;
    Vector3D direction = dudPos - pos;
    direction.normalize();

    animationSubset = 0;

    if (fabsf(direction.x) > fabsf(direction.y))
    {
        if (direction.x > 0.f)
        {
            flipX(true);
            animationSubset = 2;
        }

        if (direction.x < 0.f)
        {
            flipX(false);
            animationSubset = 2;
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
            animationSubset = 0;
        }
    }

    bool colidesWithHero = false;

    if (CollisionCircleCircle(pos.x, pos.y, 64, dudPos.x, dudPos.y, 30))
    {
        switch(animationSubset)
        {
            case 0 : animationSubset = 3; break;
            case 1 : animationSubset = 4; break;
            case 2 : animationSubset = 5; break;
        }

        colidesWithHero = true;

    }

    Vector3D newPos = pos + direction;
    
     if (!Actor::isColiding(newPos, nullptr, *map) && !actors->isColidingWithOthers(this, direction) && !colidesWithHero)
    {
        pos = newPos;
    }


    animationProgress += deltaTime * 4.f;

    if (animationProgress >= 1.f)
    {
        ++animationFrame;
        animationProgress = 0.f;

        if (animationFrame > 3)
        {
            if (animationSubset >= 3 
                && animationSubset <= 5 
                && CollisionCircleCircle(pos.x, pos.y, 64, dudPos.x, dudPos.y, 30))
            {
                dude->setHealth(dude->getHealth() - 20);
            }
            animationFrame = 0;
        }
    }


    //item pickup
    

     for (int i = 0; i < map->getItemCount(); ++i)
    {
        ItemInstance* itm = map->getItem(i);

        if (itm->isRemoved())
        {
            continue;
        }

        if (CollisionCircleCircle(pos.x + collisionBodyOffset.x,
                                  pos.y + collisionBodyOffset.y,
                                  32,
                                  itm->getPosition()->x,
                                  itm->getPosition()->y, 8))
        {
            if (itm->getIndex() == 23)//syringe needles;
            {
                damage(10);
                itm->setAsRemoved();
            }

        }
    }

}
