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
                    Actor();
    virtual         ~Actor();
    void            init(Room* currentRoom, GameMap* currentMap);
    void            destroy();
    void            dropLoot(Room* room, GameMap* map);
    void            kill(){dead = true;}
    void            flipX(bool flip){flippedX = flip;}

    void            draw(float offsetX, float offsetY,
                         int scale,
                         PicsContainer& pics, 
                         bool debugInfo = false) override;

    bool            isColiding(Vector3D newPos, Vector3D* movement, GameMap& map);
    float           getCollisionBodyRadius(){return collisionBodyRadius;}
    bool            isDead(){return dead;}
    bool            isFlipedX(){return flippedX;}
    int             getAnimationSubset(){return animationSubset;}
    virtual int     getType(){return -1;}
    void            setHealth(float newHealth);
    void            damage(float dmg);
    void            updateDamage(float deltaTime);
protected:
    void            calcCollisionResponce(Vector3D* movement,
                                          float lineX1, float lineX2,
                                          float lineY1, float lineY2);

protected:
    Room*            roomIsIn;
    GameMap*         mapIsIn;
    DArray<FrameSet> animations;
    DArray<int>      loot;
    unsigned         pictureIndex;
    float            collisionBodyRadius;
    float            health;
    float            animationProgress;
    float            damageProgress;
    int              animationFrame;
    int              animationSubset;
    bool             flippedX;
    bool             dead;
    bool             isDamaged;

};

#endif //ACTOR_H

