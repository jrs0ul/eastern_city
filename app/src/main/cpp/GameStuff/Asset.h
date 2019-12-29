#ifndef ASSET_H
#define ASSET_H

struct Asset
{
    char name[256];
    Vector3D pos;
    unsigned spriteIndex;
    int containerIndex;
};


#endif //ASSET_H
