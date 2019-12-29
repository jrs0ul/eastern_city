#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "../TextureLoader.h"
#include "GlobalItemList.h"
#include "ItemDatabase.h"
#include "ItemInstance.h"
#include "ItemContainer.h"
#include "GameMapGraph.h"
#include "Asset.h"
#include "Region.h"


class GameMap
{
public:
    GameMap();
    void        create();
    void        destroy();
#ifdef __ANDROID__
    void load(const char* file,  AAssetManager* assman, GlobalItemList* worldItems = nullptr, Room* room = nullptr);
#else
    void load(const char* filename, GlobalItemList* worldItems = nullptr, Room* room = nullptr);
#endif

    void        save(const char* file);

    void        draw(float posX, float posY, 
                     PicsContainer& pics, 
                     ItemDatabase& itemDb, 
                     bool debugInfo = false);
    void        addItem(ItemInstance* item);
    int         canTraverse(int x, int y);
    bool*       getCollisionData(){return collision;}
    Asset*      getClickedAsset(float mapOffsetX, float mapOffsetY,
                                PicsContainer& pics,
                                int x, int y);
    void        setCollision(int x, int y, bool bColide);
    Vector3D*   getPlayerPos(unsigned index);
    int         getRegionCount(){return regions.count();}
    Region*     getRegion(int index){return &regions[index];}
    int         getItemCount(){return items.count();}
    int         getItemContainerCount(){return containers.count();}
    ItemInstance* getItem(int index){return items[index];}
    ItemContainer* getItemContainer(unsigned index);
    int         getTemperature(){return temperature;}
    unsigned    getWidth(){return width;}
    unsigned    getHeight(){return height;}

private:

    DArray<ItemInstance*> items;
    DArray<Asset>         assets;
    DArray<Region>        regions;
    DArray<Vector3D>      entries;
    DArray<ItemContainer> containers;
    bool*                 collision;

    int                   temperature;

    unsigned              width;
    unsigned              height;

};


#endif //GAME_MAP_H
