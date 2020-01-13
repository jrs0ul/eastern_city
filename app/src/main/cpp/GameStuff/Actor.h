#ifndef ACTOR_H
#define ACTOR_H

#include "GameMap.h"
#include "../Vectors.h"

struct FrameSet
{
    DArray<int> frames;
    float offsetX;
    float offsetY;

    FrameSet()
    : offsetX(0.f)
    , offsetY(0.f)
    {}
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
    bool            isColiding(Vector3D newPos, Vector3D* movement, GameMap& map);
    virtual int     getType(){return -1;}
    void            setHealth(float newHealth);
protected:
    void            calcCollisionResponce(Vector3D* movement,
                                          float lineX1, float lineX2,
                                          float lineY1, float lineY2);

public:
    DArray<FrameSet> animations;
    char             spriteName[256];
    Vector3D         pos;
    Vector3D         collisionBodyOffset;
    float            collisionBodyRadius;
    float            health;
    float            animationProgress;
    float            damageProgress;
    int              animationFrame;
    int              animationSubset;
    bool             isFlipedX;
    bool             isDead;
    bool             isDamaged;

};

#endif //ACTOR_H

