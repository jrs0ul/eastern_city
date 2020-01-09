#ifndef GAME_MAP_GRAPH_H
#define GAME_MAP_GRAPH_H

#include "../DArray.h"
#include "ItemContainer.h"
#include "Asset.h"
#include "Region.h"
#include "Furniture.h"

struct RoomAndEntry;

struct CollisionTile
{
    unsigned x;
    unsigned y;
    bool     collides;
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
    void              addAsset(Vector3D pos, const char* name, unsigned spriteIndex);
    void              addFurniture(Furniture* f);
    void              addCollision(unsigned x, unsigned y, bool isColliding);
    void              addRegion(Vector3D pos, Vector3D size);
    void              addEntry(Vector3D pos);
    void              removeItem(unsigned index);
    unsigned          getItemCount();
    unsigned          getItemContainerCount();
    unsigned          getEnemyCount();
    unsigned          getChildRoomCount();
    unsigned          getAssetCount();
    unsigned          getFurnitureCount();
    unsigned          getAdditionalCollisionCount();
    unsigned          getAdditionalRegionsCount();
    unsigned          getAdditionalEntriesCount();
    ItemInstance*     getItem(unsigned index);
    ItemContainer*    getItemContainer(unsigned index);
    Vector3D*         getEnemyPosition(unsigned index);
    RoomAndEntry*     getChildRoom(unsigned index);
    RoomAndEntry*     findChildRoomByRegionIndex(unsigned regionIndex);
    Asset*            getAsset(unsigned index);
    Furniture*        getFurniture(unsigned index);
    CollisionTile*    getAdditionalCollisionTile(unsigned index);
    Region*           getAdditionalRegion(unsigned index);
    Vector3D*         getAdditionalEntry(unsigned index);
    const char*       getMapName();
    void              setMapName(const char* name);
    void              destroy(Room* parent, int level = 0);

private:
    DArray<RoomAndEntry>  childRooms;
    
    //data
    DArray<ItemContainer>  itemContainers;
    DArray<ItemInstance>   items;
    DArray<Vector3D>       enemies;
    DArray<Asset>          assets;
    DArray<Furniture>      furniture;
    DArray<CollisionTile>  additionalCollision;
    DArray<Region>         additionalRegions;
    DArray<Vector3D>       additionalEntries;
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

    void addDoorway(Room* floor,
                    unsigned x,
                    unsigned y,
                    unsigned doorRegion,
                    unsigned entryPoint,
                    unsigned returnPoint,
                    unsigned regionOffsetY,
                    Room*    destination,
                    unsigned assetIndex,
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
