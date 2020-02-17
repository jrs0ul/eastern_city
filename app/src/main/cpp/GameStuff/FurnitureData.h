#ifndef FURNITURE_DATA_H
#define FURNITURE_DATA_H

#include "../DArray.h"

struct FurnitureData
{
    char     name[128];
    unsigned index;
    unsigned damageToAxe;
    int      hp;
    unsigned spriteIndex;
    unsigned pictureIndex;
};

class FurnitureDatabase
{
public:
    void           load(const char* path);
    unsigned       getFurnitureCount(){return data.count();}
    FurnitureData* getFurniture(unsigned index);
    void           destroy();
private:
    DArray<FurnitureData> data;
};


#endif //FURNITURE_DATA_H
