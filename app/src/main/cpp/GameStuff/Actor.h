#ifndef ACTOR_H
#define ACTOR_H

#include "GameMap.h"
#include "../Vectors.h"

struct FrameSet
{
    DArray<int> frames;
};

class GameMap;

class Actor
{
public:
    virtual         ~Actor();
    void            destroy();
    void            kill(){isDead = true;}
    void            draw(float offsetX, float offsetY,
                         PicsContainer& pics, 
                         bool debugInfo = false);
    bool            isColiding(Vector3D newPos, GameMap& map);
    virtual int     getType(){return -1;}

    DArray<FrameSet> animations;
    char             spriteName[256];
    Vector3D         pos;
    Vector3D         collisionBodyOffset;
    float            collisionBodyRadius;
    float            health;
    float            animationProgress;
    int              animationFrame;
    int              animationSubset;
    bool             isFlipedX;
    bool             isDead;

};

#endif //ACTOR_H

