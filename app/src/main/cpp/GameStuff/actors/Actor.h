#ifndef ACTOR_H
#define ACTOR_H

#include "../Drawable.h"
#include "../map/GameMap.h"
#include "../../Vectors.h"

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

class Actor : public Drawable
{
public:
    virtual         ~Actor();
    void            destroy();
    void            kill(){isDead = true;}

    void            draw(float offsetX, float offsetY,
                         int scale,
                         PicsContainer& pics, 
                         bool debugInfo = false) override;

    bool            isColiding(Vector3D newPos, Vector3D* movement, GameMap& map);
    virtual int     getType(){return -1;}
    void            setHealth(float newHealth);
protected:
    void            calcCollisionResponce(Vector3D* movement,
                                          float lineX1, float lineX2,
                                          float lineY1, float lineY2);

public:
    DArray<FrameSet> animations;
    unsigned         pictureIndex;
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

