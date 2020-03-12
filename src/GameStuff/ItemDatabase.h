#ifndef ITEM_DB_H
#define ITEM_DB_H

#ifdef __ANDROID__
    #include <android/asset_manager.h>
#endif
#include "ItemData.h"
#include "../DArray.h"

class ItemDatabase
{
public:
#ifdef __ANDROID__
    void load(const char* filename,  AAssetManager* assman);
#else
    void load(const char* filename);
#endif

    void destroy();
    ItemData* get(unsigned index);
private:
    DArray<ItemData> items;
};

#endif //ITEM_DB_H
