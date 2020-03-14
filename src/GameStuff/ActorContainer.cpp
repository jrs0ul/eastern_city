#include "ActorContainer.h"
#include "../MathTools.h"
#include "actors/Rat.h"
#include "actors/Ghost.h"
#include "actors/Bear.h"
#include <cassert>

void ActorContainer::destroy()
{
    for (unsigned i = 0; i < actorsToDelete.count(); ++i)
    {
        if (actorsToDelete[i])
        {
            delete actorsToDelete[i];
        }
    }
   
    actorsToDelete.destroy();
    actors.destroy();
}

void ActorContainer::addActor(Actor* actor)
{
    assert(actor);
    actors.add(actor);
}

void ActorContainer::createAndAddActor(Vector3D& pos,
                                       unsigned type, 
                                       Room* room, 
                                       GameMap* map)
{
    switch (type)
    {
        case 1: {
                    Rat* rat = new Rat();
                    rat->init(pos, room, map);
                    actors.add(rat);
                    actorsToDelete.add(rat);
                } break;
        case 2: {
                    Ghost* ghost = new Ghost();
                    ghost->init(pos, room, map);
                    actors.add(ghost);
                    actorsToDelete.add(ghost);
                } break;

        case 3: {
                    Bear* bear = new Bear();
                    bear->init(pos, room, map);
                    actors.add(bear);
                    actorsToDelete.add(bear);
                } break;
        default: break;
    }

}

void ActorContainer::draw(float offsetX, float offsetY,
                          int scale,
                          PicsContainer& pics,
                          DArray<Furniture*>& furniture,
                          bool debug)
{


    

    DArray<Drawable*> renderUnits;

    for (unsigned i = 0; i < actors.count(); ++i)
    {
        Drawable* dude = static_cast<Drawable*>(actors[i]);
        
        if (dude && actors[i]->isDead())
        {
            continue;
        }

        renderUnits.add(dude);
    }

    for (unsigned i = 0; i < furniture.count(); ++i)
    {
        renderUnits.add(furniture[i]);
    }


    //sorts by Y
    while (renderUnits.count())
    {

        float minY = 800000;
        int smallestYIndex = -1;

        for (int i = 0; i < (int)renderUnits.count(); ++i)
        {
            if (!renderUnits[i])
            {
                continue;
            }

            float currentY = renderUnits[i]->pos.y + renderUnits[i]->collisionBodyOffset.y;

            if ( currentY < minY)
            {
                minY = currentY;
                smallestYIndex = i;
            }
        }

        /*printf("%f + %f = %f\n", renderUnits[smallestYIndex]->pos.y, 
                                 renderUnits[smallestYIndex]->collisionBodyOffset.y,
                                 minY);*/

        renderUnits[smallestYIndex]->draw(offsetX, offsetY, scale, pics, debug);
        renderUnits.remove(smallestYIndex);

    }
    //printf("-------------\n");

    renderUnits.destroy();
}

unsigned ActorContainer::getActorCount()
{
    return actors.count();
}

Actor* ActorContainer::getActor(unsigned index)
{
    if (index < actors.count())
    {
        return actors[index];
    }

    assert(true);

    return nullptr;
}

bool ActorContainer::isColidingWithOthers(Actor* actor, Vector3D& offset)
{
    Vector3D newPosition = Vector3D(actor->pos.x + offset.x + actor->collisionBodyOffset.x,
                                    actor->pos.y + offset.y + actor->collisionBodyOffset.y,
                                    0);

    for (unsigned i = 0; i < actors.count(); ++i)
    {
        Actor* pActor = actors[i];

        if (pActor == actor || pActor->isDead())
        {
            continue;
        }

        if (CollisionCircleCircle(newPosition.x,
                                  newPosition.y,
                                  actor->getCollisionBodyRadius(),
                                  actors[i]->pos.x + actors[i]->collisionBodyOffset.x, 
                                  actors[i]->pos.y + actors[i]->collisionBodyOffset.y, 
                                  actors[i]->getCollisionBodyRadius()))
        {
            return true;
        }
    }

    return false;
}

