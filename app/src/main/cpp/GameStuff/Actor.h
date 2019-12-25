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
    void    destroy();
    void    draw(float offsetX, float offsetY,
                 PicsContainer& pics, 
                 bool debugInfo = false);
    bool    isColiding(Vector3D newPos, GameMap& map);

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

};

#endif //ACTOR_H

