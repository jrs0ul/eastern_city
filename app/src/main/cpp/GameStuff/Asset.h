#ifndef ASSET_H
#define ASSET_H

struct Asset
{
    char name[256];
    Vector3D pos;
    unsigned spriteIndex;
    int containerIndex;
    bool isBed;

    Asset()
    : spriteIndex(0)
    , containerIndex(-1)
    , isBed(false)
    {
    }
};


#endif //ASSET_H
