#ifndef FURNITURE_DATA_H
#define FURNITURE_DATA_H

#include "../Xml.h"
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
#ifndef __ANDROID__
    void load(const char* path);
#else
    void load(const char* path, AAssetManager* assman);
#endif
    unsigned       getFurnitureCount(){return data.count();}
    FurnitureData* getFurniture(unsigned index);
    void           destroy();
private:
    DArray<FurnitureData> data;
};


#endif //FURNITURE_DATA_H
