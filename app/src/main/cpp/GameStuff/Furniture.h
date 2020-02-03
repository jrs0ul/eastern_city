#ifndef FURNITURE_H
#define FURNITURE_H

#include "Drawable.h"
#include "Polygon.h"

class Furniture : public Drawable
{
public:
    Furniture()
    : Drawable()
    , collisionBodyPos(0, 0, 0)
    , itemContainerIndex(-1)
    , colidedWithHero(false)
    , isBed(false)
    , removed(false)
    {
    }

    void destroy();
    void draw(float offsetX, float offsetY,
              PicsContainer& pics,
              bool debugInfo = false);
    //void test(){}


    SPolygon collisionPolygon;
    Vector3D collisionBodyPos;
    Vector3D collisionBodySize;
    int      spriteIndex;
    int      pictureIndex;
    int      itemContainerIndex;
    int      furnitureDbIndex;
    bool     colidedWithHero;
    bool     isBed;
    bool     removed;
};


#endif //FURNITURE_H
