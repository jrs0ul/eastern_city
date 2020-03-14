#ifndef GHOST_H
#define GHOST_H

#include "Actor.h"
#include "Dude.h"
#include "../ActorContainer.h"

class Ghost : public Actor
{
public:
    void init(Vector3D& position, Room* currentRoom, GameMap* currentMap);
    void update(float deltaTime, GameMap* map, Actor* dude, ActorContainer* actors);

    virtual int getType() override {return 2;}
};


#endif //GHOST_H
