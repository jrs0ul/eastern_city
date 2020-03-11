#ifndef BEAR_H
#define BEAR_H

#include "Actor.h"
#include "Dude.h"

class Bear : public Actor
{
public:
    void init(Vector3D& position);
    void update(float delaTime, GameMap& map, Dude& dude, ActorContainer& actors);

    virtual int getType() override {return 3;}
};

#endif //BEAR_H
