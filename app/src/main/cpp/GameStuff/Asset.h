#ifndef ASSET_H
#define ASSET_H

struct Asset
{
    char name[256];
    Vector3D pos;
    unsigned spriteIndex;

    Asset()
    : spriteIndex(0)
    {
    }
};


#endif //ASSET_H
