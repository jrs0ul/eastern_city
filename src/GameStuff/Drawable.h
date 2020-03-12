#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "../TextureLoader.h"
#include "../Vectors.h"

class Drawable
{
public:
    virtual ~Drawable(){}
    virtual void draw(float offsetX, float offsetY,
                      int scale,
                      PicsContainer& pics,
                      bool debugInfo = false) = 0;

    Vector3D pos;
    Vector3D collisionBodyOffset;
};


#endif
