#ifndef RAT_H
#define RAT_H

#include "Actor.h"
#include "Dude.h"
#include "../ActorContainer.h"

class Rat : public Actor
{
public:
    void init(Vector3D& position, Room* currentRoom, GameMap* currentMap);
    void update(float deltaTime, GameMap& map, Dude& dude, ActorContainer& actors);

    virtual int getType() override {return 1;}
};

#endif //RAT_H
