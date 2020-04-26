#ifndef ASSET_H
#define ASSET_H

#include "../Vectors.h"

struct Asset
{
    char name[256];
    Vector3D pos;
    unsigned spriteIndex;
    bool isFlipped;
    bool isLight;
    bool isInFrontLayer;

    Asset()
    : spriteIndex(0)
    , isFlipped(false)
    , isLight(false)
    , isInFrontLayer(false)
    {
    }
};


#endif //ASSET_H
