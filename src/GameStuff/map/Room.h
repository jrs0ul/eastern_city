#ifndef ROOM_H
#define ROOM_H

#include "../Furniture.h"
#include "../Asset.h"
#include "AdditionalVertices.h"
#include "Region.h"
#include "../ItemContainer.h"

class Room;
class Actor;

struct RoomAndEntry
{
    Room* room;
    unsigned entryIndex;
    unsigned region;
};

struct EnemyPos
{
    Vector3D position;
    unsigned type;
};

struct DoorHole
{
    Vector3D hole;
    float    topX1Offset;
};


class Room
{
public:
    
                      Room();
                      ~Room();
    static int        getExistingRooms();

    void              addChildRoom(const char* name, unsigned entry, unsigned region);
    void              addChildRoom(Room* node, unsigned entry, unsigned region);
    void              addItem(Vector3D pos, int index);
    void              addItem(Vector3D pos, ItemInstance* item);
    void              addItemContainer(unsigned furnitureIndex, ItemContainer& container);
    void              addRespawningEnemy(Vector3D pos, unsigned type);
    void              addConstantEnemy(Vector3D pos, unsigned type);
    void              addAsset(Vector3D pos, 
                               const char* name,
                               unsigned spriteIndex,
                               bool flipped = false,
                               bool frontLayer = false);
    void              addFurniture(Furniture& f);
    void              addCollisionPolygon(SPolygon& p);
    void              addDoorHole(float x1, float x2, float height = 50.f, float topX1Offset = 0.f);
    void              addRegion(Vector3D pos, Vector3D size);
    void              addEntry(Vector3D pos);
    void              addVerticesAfter(SPolygon& p, unsigned indexToPutVerticesAfter);
    void              removeItem(unsigned index);
    unsigned          getItemCount();
    unsigned          getItemContainerCount();
    unsigned          getRespawningEnemyCount();
    unsigned          getConstantEnemyCount();
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
    EnemyPos*         getRespawningEnemy(unsigned index);
    Actor*            getConstantEnemy(unsigned index);
    RoomAndEntry*     getChildRoom(unsigned index);
    RoomAndEntry*     findChildRoomByRegionIndex(unsigned regionIndex);
    Asset*            getAsset(unsigned index);
    Furniture*        getFurniture(unsigned index);
    Region*           getAdditionalRegion(unsigned index);
    Vector3D*         getAdditionalEntry(unsigned index);
    SPolygon*          getCollisionPolygon(unsigned index);
    AdditionalVertices* getAdditionalVertices(unsigned index);
    DoorHole*         getDoorHole(unsigned index);
    const char*       getMapName();
    void              setMapName(const char* name);
    void              destroy(Room* parent, int level = 0);

private:
    DArray<RoomAndEntry>  childRooms;
    
    //data
    DArray<ItemContainer>       itemContainers;
    DArray<ItemInstance>        items;
    DArray<EnemyPos>            respawningEnemies;
    DArray<Actor*>              constantEnemies;
    DArray<Asset>               assets;
    DArray<Furniture*>          furniture;
    DArray<SPolygon>            collisionPolygons;
    DArray<DoorHole>            doorHoles;
    DArray<Region>              additionalRegions;
    DArray<Vector3D>            additionalEntries;
    DArray<AdditionalVertices>  additionalVertices;
    char mapName[256];
};

#endif //ROOM_H
