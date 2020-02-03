#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "../TextureLoader.h"
#include "GlobalItemList.h"
#include "ItemDatabase.h"
#include "ItemInstance.h"
#include "ItemContainer.h"
#include "GameMapGraph.h"
#include "Polygon.h"
#include "Actor.h"
#include "Asset.h"
#include "Region.h"
#include "Furniture.h"

class Actor;


class GameMap
{
public:
    GameMap();
    void        destroy();
#ifdef __ANDROID__
    void load(const char* file,  AAssetManager* assman, GlobalItemList* worldItems = nullptr, Room* room = nullptr);
#else
    void load(const char* filename, GlobalItemList* worldItems = nullptr, Room* room = nullptr);
#endif

    void        save(const char* file);

    void        draw(float posX, float posY,
                     unsigned screenWidth, unsigned screenHeight,
                     PicsContainer& pics, 
                     ItemDatabase& itemDb, 
                     bool debugInfo = false);

    void        update(Actor* mainguy, PicsContainer& pics);
    void        updateFurniturePolygons(Room* currentRoom);

    void        drawFrontLayerAssets(float offsetX, float offsetY,
                                     Vector3D& characterPos,
                                     PicsContainer& pics);
    void        drawDarknessBorder(float offsetX, float offsetY,
                                   unsigned screenWidth, unsigned screenHeight,
                                   PicsContainer& pics);
    void        addItem(ItemInstance* item);
    Furniture*  getClickedFurniture(float mapOffsetX, float mapOffsetY,
                                PicsContainer& pics,
                                int x, int y,
                                bool returnIfColidesWithHero,
                                float heroX = 0.f, float heroY = 0.f);

    unsigned    getPolygonCount(){return polygons.count();}
    SPolygon*    getPolygon(unsigned index){
                    return (index < polygons.count()) ? &polygons[index] : nullptr;
                }
    unsigned    getAdditionalPathPointCount(){return addititionalNodePoints.count();}
    Vector3D*   getAdditionalPathPoints(){return (Vector3D*)addititionalNodePoints.getData();}
    SPolygon*    getPolygonData(){return (SPolygon*)polygons.getData();}
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
    
    DArray<Furniture*>&   getFurnitureData(){return furniture;}

private:
    void        createDoorHole(float x1, float x2, float height);


private:

    DArray<ItemInstance*>  items;
    DArray<Asset>          assets;
    DArray<Region>         regions;
    DArray<Vector3D>       entries;
    DArray<ItemContainer*> containers;
    DArray<Furniture*>     furniture;

    DArray<SPolygon>        polygons;
    DArray<Vector3D>       addititionalNodePoints;

    int                    temperature;
    unsigned               polygonsBeforeFurnitureAdded;

    unsigned               width;
    unsigned               height;

};


#endif //GAME_MAP_H
