#include "ActorContainer.h"
#include "../Usefull.h"

void ActorContainer::destroy()
{
    actors.destroy();
}

void ActorContainer::addActor(Actor& actor)
{
    actors.add(actor);
}

void ActorContainer::draw(float offsetX, float offsetY, 
                          PicsContainer& pics,
                          bool debug)
{

    DArray<Actor*> renderUnits;

    for (unsigned i = 0; i < actors.count(); ++i)
    {
        Actor* dude = &actors[i];
        renderUnits.add(dude);
    }

    //sorts by Y
    while (renderUnits.count())
    {

        float minY = 800000;
        int smallestYIndex = -1;

        for (int i = 0; i < (int)renderUnits.count(); ++i)
        {
            float currentY = renderUnits[i]->pos.y;

            if ( currentY < minY)
            {
                minY = currentY;
                smallestYIndex = i;
            }
        }

        renderUnits[smallestYIndex]->draw(offsetX, offsetY, pics, debug);
        renderUnits.remove(smallestYIndex);

    }

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
        return &actors[index];
    }

    return nullptr;
}

bool ActorContainer::isColidingWithOthers(Actor* actor, Vector3D& offset)
{
    for (unsigned i = 0; i < actors.count(); ++i)
    {
        Actor* pActor = &actors[i];

        if (pActor == actor)
        {
            return false;
        }

        if (CollisionCircleCircle(actor->pos.x + offset.x, actor->pos.y + offset.y, actor->collisionBodyRadius,
                                  actors[i].pos.x, actors[i].pos.y, actors[i].collisionBodyRadius))
        {
            return true;
        }
    }

    return false;
}

