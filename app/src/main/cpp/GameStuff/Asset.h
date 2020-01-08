#ifndef ASSET_H
#define ASSET_H

struct Asset
{
    char name[256];
    Vector3D pos;
    unsigned spriteIndex;
    int containerIndex;
    bool interactable;
    bool isBed;
    bool colidedWithHero;

    Asset()
    : spriteIndex(0)
    , containerIndex(-1)
    , interactable(false)
    , isBed(false)
    , colidedWithHero(false)
    {
    }
};


#endif //ASSET_H
