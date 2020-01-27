#ifndef ASSET_H
#define ASSET_H

struct Asset
{
    char name[256];
    Vector3D pos;
    unsigned spriteIndex;
    bool isFlipped;
    bool isInFrontLayer;

    Asset()
    : spriteIndex(0)
    , isFlipped(false)
    , isInFrontLayer(false)
    {
    }
};


#endif //ASSET_H
