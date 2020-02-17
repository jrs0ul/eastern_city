#ifndef FURNITURE_H
#define FURNITURE_H

#include "Drawable.h"
#include "Polygon.h"

struct FurnitureData;

class Furniture : public Drawable
{
public:
    Furniture()
    : Drawable()
    , collisionBodyPos(0, 0, 0)
    , itemContainerIndex(-1)
    , hp(0)
    , colidedWithHero(false)
    , isBed(false)
    , removed(false)
    {
    }

    void initFromData(FurnitureData* fd);
    void destroy();
    void draw(float offsetX, float offsetY,
              int scale,
              PicsContainer& pics,
              bool debugInfo = false);


    SPolygon collisionPolygon;
    Vector3D collisionBodyPos;
    Vector3D collisionBodySize;
    int      spriteIndex;
    int      pictureIndex;
    int      itemContainerIndex;
    int      hp;
    int      furnitureDbIndex;
    bool     colidedWithHero;
    bool     isBed;
    bool     removed;
};


#endif //FURNITURE_H
