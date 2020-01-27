#ifndef GAME_MAP_GRAPH_H
#define GAME_MAP_GRAPH_H

#include "../DArray.h"
#include "ItemContainer.h"
#include "Asset.h"
#include "Region.h"
#include "Polygon.h"
#include "Furniture.h"

struct RoomAndEntry;

struct AdditionalVertices
{
    Polygon p;
    unsigned index;
};

class Room
{
public:
    void              addChildRoom(const char* name, unsigned entry, unsigned region);
    void              addChildRoom(Room* node, unsigned entry, unsigned region);
    void              addItem(Vector3D pos, int index);
    void              addItem(Vector3D pos, ItemInstance* item);
    void              addItemContainer(unsigned furnitureIndex, ItemContainer& container);
    void              addEnemyPosition(Vector3D pos);
    void              addAsset(Vector3D pos, 
                               const char* name,
                               unsigned spriteIndex,
                               bool flipped = false,
                               bool frontLayer = false);
    void              addFurniture(Furniture* f);
    void              addCollisionPolygon(Polygon& p);
    void              addDoorHole(float x1, float x2, float height = 50.f);
    void              addRegion(Vector3D pos, Vector3D size);
    void              addEntry(Vector3D pos);
    void              addVerticesAfter(Polygon& p, unsigned indexToPutVerticesAfter);
    void              removeItem(unsigned index);
    unsigned          getItemCount();
    unsigned          getItemContainerCount();
    unsigned          getEnemyCount();
    unsigned          getChildRoomCount();
    unsigned          getAssetCount();
    unsigned          getFurnitureCount();
    unsigned          getAdditionalRegionsCount();
    unsigned          getAdditionalEntriesCount();
    unsigned          getCollisionPolygonCount();
    unsigned          getAdditionalVerticesCount(){return additionalVertices.count();}
    unsigned          getDoorHoleCount();
    ItemInstance*     getItem(unsigned index);
    ItemContainer*    getItemContainer(unsigned index);
    Vector3D*         getEnemyPosition(unsigned index);
    RoomAndEntry*     getChildRoom(unsigned index);
    RoomAndEntry*     findChildRoomByRegionIndex(unsigned regionIndex);
    Asset*            getAsset(unsigned index);
    Furniture*        getFurniture(unsigned index);
    Region*           getAdditionalRegion(unsigned index);
    Vector3D*         getAdditionalEntry(unsigned index);
    Polygon*          getCollisionPolygon(unsigned index);
    AdditionalVertices* getAdditionalVertices(unsigned index);
    Vector3D*         getDoorHole(unsigned index);
    const char*       getMapName();
    void              setMapName(const char* name);
    void              destroy(Room* parent, int level = 0);

private:
    DArray<RoomAndEntry>  childRooms;
    
    //data
    DArray<ItemContainer>       itemContainers;
    DArray<ItemInstance>        items;
    DArray<Vector3D>            enemies;
    DArray<Asset>               assets;
    DArray<Furniture>           furniture;
    DArray<Polygon>             collisionPolygons;
    DArray<Vector3D>            doorHoles;
    DArray<Region>              additionalRegions;
    DArray<Vector3D>            additionalEntries;
    DArray<AdditionalVertices>  additionalVertices;
    char mapName[256];
};

struct RoomAndEntry
{
    Room* room;
    unsigned entryIndex;
    unsigned region;
};



class GameMapGraph
{
public:
    GameMapGraph(): root(nullptr){}

    void init();

    Room* addFloors(Room* mainfloor, unsigned entranceIndex);

    void addTunnelLeft(Room* room);
    void addTunnelRight(Room* room);
    void addTunnelBottom(Room* room);
    void addFrontBuidingDoor(Room* room);
    void addLeftBuildingDoor(Room* room);

    void addBuilding(Room* outside, unsigned regionIndex);

    void addDoorway(Room* floor,
                    unsigned x,
                    unsigned y,
                    unsigned doorRegion,
                    unsigned entryPoint,
                    unsigned returnPoint,
                    unsigned regionOffsetY,
                    Room*    destination,
                    unsigned assetIndex,
                    float doorWidth = 64.f,
                    float doorwayOffsetX = 0.f,
                    bool isLeft = true);
    
    void generateRoom(Room* floor, 
                      unsigned entryPoint, 
                      unsigned doorRegion,
                      unsigned returnPoint,
                      bool isLeft = true);

    void addFridge(Room* room);
    void addTvCupboard(Room* room);
    void addCupboard(Room* room);
    void addCouch(Room* room, int x, int y);

    void destroy();


    Room* root;
};

#endif //GAME_MAP_GRAPH_H
