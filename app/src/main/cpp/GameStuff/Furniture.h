#ifndef FURNITURE_H
#define FURNITURE_H

struct Furniture
{
    Furniture()
    : collisionBodyPos(0, 0, 0)
    , itemContainerIndex(-1)
    , colidedWithHero(false)
    , isBed(false)
    , removed(false)
    {
    }

    Vector3D pos;
    Vector3D collisionBodyPos;
    Vector3D collisionBodySize;
    int      spriteIndex;
    int      pictureIndex;
    int      itemContainerIndex;
    bool     colidedWithHero;
    bool     isBed;
    bool     removed;
};


#endif //FURNITURE_H
