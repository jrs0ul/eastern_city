#ifndef REGION_H
#define REGION_H

struct Region
{
    char name[256];
    Vector3D pos;
    Vector3D size;
    unsigned entryIndex;
};


#endif //REGION_H
