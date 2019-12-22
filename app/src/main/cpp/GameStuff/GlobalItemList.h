#ifndef GLOBAL_ITEM_LIST_H
#define GLOBAL_ITEM_LIST_H

#include "ItemInstance.h"

struct ItemGroup
{
    char mapName[256];
    DArray<ItemInstance> items;
};


class GlobalItemList
{
public:
#ifdef __ANDROID__
    void load(const char* filename,  AAssetManager* assman);
#else
    void load(const char* filename);
#endif
    void          destroy();
    ItemInstance* get(const char* mapname, unsigned index);
    unsigned      getItemCountInMap(const char* mapname);
    void          remove(const char* mapname, unsigned index);
private:
    DArray<ItemGroup> itemList;
};


#endif //GLOBAL_ITEM_LIST_H
