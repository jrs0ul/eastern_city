#ifndef GAME_MAP_GRAPH_H
#define GAME_MAP_GRAPH_H

#include "../DArray.h"
#include "ItemInstance.h"
#include "Asset.h"
#include "Region.h"

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
    void              addAsset(Vector3D pos, const char* name, unsigned spriteIndex);
    void              addCollision(unsigned x, unsigned y, bool isColliding);
    void              addRegion(Vector3D pos, Vector3D size);
    void              addEntry(Vector3D pos);
    void              removeItem(unsigned index);
    unsigned          getItemCount();
    unsigned          getChildRoomCount();
    unsigned          getAssetCount();
    unsigned          getAdditionalCollisionCount();
    unsigned          getAdditionalRegionsCount();
    unsigned          getAdditionalEntriesCount();
    ItemInstance*     getItem(unsigned index);
    RoomAndEntry*     getChildRoom(unsigned index);
    RoomAndEntry*     findChildRoomByRegionIndex(unsigned regionIndex);
    Asset*            getAsset(unsigned index);
    CollisionTile*    getAdditionalCollisionTile(unsigned index);
    Region*           getAdditionalRegion(unsigned index);
    Vector3D*         getAdditionalEntry(unsigned index);
    const char*       getMapName();
    void              setMapName(const char* name);
    void              destroy(Room* parent, int level = 0);

private:
    DArray<RoomAndEntry>  childRooms;
    
    //data
    DArray<ItemInstance>  items;
    DArray<Asset>         assets;
    DArray<CollisionTile> additionalCollision;
    DArray<Region>        additionalRegions;
    DArray<Vector3D>      additionalEntries;
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

    void destroy();


    Room* root;
};

#endif //GAME_MAP_GRAPH_H
