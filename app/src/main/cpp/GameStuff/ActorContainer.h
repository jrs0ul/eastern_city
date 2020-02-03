#ifndef ACTOR_CONTAINER_H
#define ACTOR_CONTAINER_H

#include "../DArray.h"
#include "Actor.h"
#include "Furniture.h"
#include "../TextureLoader.h"

class ActorContainer
{
public:
    void     destroy();
    void     addActor(Actor* actor);
    void     draw(float offsetX, float offsetY, PicsContainer& pics, 
                  DArray<Furniture*>& furniture,
                  bool debug);
    unsigned getActorCount();
    Actor*   getActor(unsigned index);
    bool     isColidingWithOthers(Actor* actor, Vector3D& offset);


private:

    DArray<Actor*> actors;
};


#endif //ACTOR_CONTAINER_H
