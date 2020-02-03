#include "ActorContainer.h"
#include "../Useful.h"

void ActorContainer::destroy()
{
    for (unsigned i = 0; i < actors.count(); ++i)
    {
        if (actors[i]->getType())//don't delete main character
        {
            delete actors[i];
        }
    }
    actors.destroy();
}

void ActorContainer::addActor(Actor* actor)
{
    actors.add(actor);
}

void ActorContainer::draw(float offsetX, float offsetY, 
                          PicsContainer& pics,
                          DArray<Furniture*>& furniture,
                          bool debug)
{


    

    DArray<Drawable*> renderUnits;

    for (unsigned i = 0; i < actors.count(); ++i)
    {
        Drawable* dude = static_cast<Drawable*>(actors[i]);
        
        if (actors[i]->isDead)
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

        renderUnits[smallestYIndex]->draw(offsetX, offsetY, pics, debug);
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

        if (pActor == actor || pActor->isDead)
        {
            continue;
        }

        if (CollisionCircleCircle(newPosition.x,
                                  newPosition.y,
                                  actor->collisionBodyRadius,
                                  actors[i]->pos.x + actors[i]->collisionBodyOffset.x, 
                                  actors[i]->pos.y + actors[i]->collisionBodyOffset.y, 
                                  actors[i]->collisionBodyRadius))
        {
            return true;
        }
    }

    return false;
}

