#ifndef RAT_H
#define RAT_H

#include "Actor.h"

class ActorContainer;

class Rat : public Actor
{
public:
    void init(Vector3D& position, Room* currentRoom, GameMap* currentMap);
    void update(float deltaTime, GameMap* map, Actor* dude, ActorContainer* actors);

    virtual int getType() override {return 1;}
};

#endif //RAT_H
