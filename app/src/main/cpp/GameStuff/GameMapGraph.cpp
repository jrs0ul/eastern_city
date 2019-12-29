#include "GameMapGraph.h"

void Room::addChildRoom(const char* name, unsigned entry, unsigned region)
{
    RoomAndEntry entr;
    entr.room = new Room();
    entr.room->setMapName(name);
    entr.entryIndex = entry;
    entr.region = region;

    childRooms.add(entr);
}

void Room::addChildRoom(Room* room, unsigned entry, unsigned region)
{
    RoomAndEntry entr;
    entr.room = room;
    entr.entryIndex = entry;
    entr.region = region;
    childRooms.add(entr);
}

void Room::addItem(Vector3D pos, int index)
{
    ItemInstance item;
    item.init(pos, index);
    items.add(item);
}

void Room::addEnemyPosition(Vector3D pos)
{
    enemies.add(pos);
}

void Room::addAsset(Vector3D pos, const char* name, unsigned spriteIndex)
{
    Asset ass;
    ass.pos = pos;
    strcpy(ass.name, name);
    ass.spriteIndex = spriteIndex;
    assets.add(ass);
}

void Room::addCollision(unsigned x, unsigned y, bool isColliding)
{
    CollisionTile tile;
    tile.x = x;
    tile.y = y;
    tile.collides = isColliding;
    additionalCollision.add(tile);
}

void Room::addRegion(Vector3D pos, Vector3D size)
{
    Region reg;
    reg.pos = pos;
    reg.size = size;
    additionalRegions.add(reg);
}

void Room::addEntry(Vector3D pos)
{
    additionalEntries.add(pos);
}

void Room::removeItem(unsigned index)
{
    if (index < items.count())
    {
        items[index].setAsRemoved();
    }
}

unsigned Room::getItemCount()
{
    return items.count();
}

unsigned Room::getEnemyCount()
{
    return enemies.count();
}

unsigned Room::getChildRoomCount()
{
    return childRooms.count();
}

unsigned Room::getAssetCount()
{
    return assets.count();
}

unsigned Room::getAdditionalCollisionCount()
{
    return additionalCollision.count();
}

unsigned Room::getAdditionalRegionsCount()
{
    return additionalRegions.count();
}

unsigned Room::getAdditionalEntriesCount()
{
    return additionalEntries.count();
}

ItemInstance* Room::getItem(unsigned index)
{
    if (index < items.count())
    {
        return &items[index];
    }

    return nullptr;
}

Vector3D* Room::getEnemyPosition(unsigned index)
{
    if (index < enemies.count())
    {
        return &enemies[index];
    }

    return nullptr;
}


RoomAndEntry* Room::getChildRoom(unsigned index)
{
    if (index < childRooms.count())
    {
        return &childRooms[index];
    }

    return nullptr;
}


RoomAndEntry* Room::findChildRoomByRegionIndex(unsigned regionIndex)
{
    for (unsigned i = 0; i < childRooms.count(); ++i)
    {
        if (childRooms[i].region == regionIndex)
        {
            return &childRooms[i];
        }
    }

    return nullptr;
}

Asset* Room::getAsset(unsigned index)
{
    if (index < assets.count())
    {
        return &assets[index];
    }

    return nullptr;
}

CollisionTile* Room::getAdditionalCollisionTile(unsigned index)
{
    if (index < additionalCollision.count())
    {
        return &additionalCollision[index];
    }

    return nullptr;

}

Region* Room::getAdditionalRegion(unsigned index)
{
    if (index < additionalRegions.count())
    {
        return &additionalRegions[index];
    }

    return nullptr;

}

Vector3D* Room::getAdditionalEntry(unsigned index)
{
    if (index < additionalEntries.count())
    {
        return &additionalEntries[index];
    }

    return nullptr;

}

const char* Room::getMapName()
{
    return mapName;
}

void  Room::setMapName(const char* name)
{
    strcpy(mapName, name);
}


void Room::destroy(Room* parent, int level)
{
    for (int i = 0; i < level; ++i)
    {
        printf("-");
    }

    printf("deleting %s [child rooms:%lu assets:%lu; parent: %s]\n", mapName, childRooms.count(), assets.count(), parent->mapName);

    for (unsigned i = 0; i < childRooms.count(); ++i)
    {
        if (childRooms[i].room != parent)
        {
            //printf("%p != %p\n", childRooms[i].room, parent);
            childRooms[i].room->destroy(this, level + 1);
            delete childRooms[i].room;
        }
    }

    for (int i = 0; i < level; ++i)
    {
        printf("-");
    }

    printf("%s children deleted\n", mapName);

    childRooms.destroy();
    items.destroy();
    enemies.destroy();
    assets.destroy();
    additionalCollision.destroy();
    additionalRegions.destroy();
    additionalEntries.destroy();
}



void GameMapGraph::init()
{
    root = new Room();
    root->setMapName("data/room.xml");

    root->addChildRoom("data/stairwell.xml", 2, 0);
    root->addItem(Vector3D(300, 260, 0), rand() % 2);
    Room* stairwell = root->getChildRoom(0)->room;

    stairwell->addChildRoom("data/outside.xml", 0, 0);
    Room* outside = stairwell->getChildRoom(0)->room;

    addDoorway(stairwell, 672, 120, 2, 1, 2, 50, root, 4);

    Room* lastFloor = addFloors(stairwell, 2);

    addDoorway(lastFloor, 288, 98, 2, 0, 2, 0, nullptr, 2);
    lastFloor->addItem(Vector3D(300, 300, 0), 8);
    lastFloor->addItem(Vector3D(360, 320, 0), 8);
    lastFloor->addItem(Vector3D(320, 340, 0), 8);
    lastFloor->addItem(Vector3D(360, 305, 0), 8);
    lastFloor->addCollision(5, 5, true);
    lastFloor->addCollision(6, 5, true);
    lastFloor->addCollision(7, 5, true);

    addDoorway(stairwell, 288, 98, 3, 0, 3, 0, nullptr, 2);
    addDoorway(stairwell, 544, 96, 4, 0, 4, 0,  nullptr, 2);

    outside->addChildRoom(stairwell, 0, 0);
    
    outside->addChildRoom("data/stairwell.xml", 0, 1);
    outside->addChildRoom("data/stairwell.xml", 0, 2);

    Room* buildingLeft = outside->getChildRoom(1)->room;
    Room* buildingRight = outside->getChildRoom(2)->room;
    buildingLeft->addChildRoom(outside, 1, 0);
    buildingRight->addChildRoom(outside, 2, 0);

    addFloors(buildingLeft, 1);
    addFloors(buildingRight, 1);


    for (int i = 0; i < 15; ++i)
    {
        outside->addEnemyPosition(Vector3D(400 + i * 32, 400 + rand() % 100, 0 ));
    }

    printf("done generating\n");
}

Room* GameMapGraph::addFloors(Room* mainfloor, unsigned entranceIndex)
{
    mainfloor->addChildRoom("data/stairwell2.xml", 0, 1);

    int additionalFloors = rand() % 3 + 1;
    printf("BUILDING FLOORS: %d\n", additionalFloors + 2);

    Room* previousFloor = mainfloor;
    Room* currentFloor = mainfloor->getChildRoom(entranceIndex)->room;;

    for (int i = 0; i < additionalFloors; ++i)
    {
        printf("floor %d\n", 1 + i);

        if (previousFloor == mainfloor)
        {
            currentFloor->addChildRoom(previousFloor, 1, 0);
        }
        else
        {
            currentFloor->addChildRoom(previousFloor, 1, 0);
        }

        currentFloor->addChildRoom("data/stairwell2.xml", 0, 1);
        currentFloor->addAsset(Vector3D(0, 0, 0), "pics/test.tga", 3); //stairs


        addDoorway(currentFloor, 288, 98, 2, 0, 2, 0, nullptr, 2);
        addDoorway(currentFloor, 672, 115, 3, 0, 3, 50, nullptr, 4);
        addDoorway(currentFloor, 40, 128, 4, 0, 4, 50, nullptr, 5, false);


        previousFloor = currentFloor;
        currentFloor = currentFloor->getChildRoom(1)->room;

    }

    Room* lastFloor = currentFloor;
    lastFloor->addChildRoom(previousFloor, 1, 0);

    return lastFloor;
}

void GameMapGraph::addDoorway(Room* floor,
                              unsigned rx,
                              unsigned ry,
                              unsigned doorRegion, 
                              unsigned entryPoint,
                              unsigned returnPoint, 
                              unsigned regionOffsetY,
                              Room*    destination, 
                              unsigned assetIndex,
                              bool isLeft)
{
    const unsigned x = rx / 32;
    const unsigned y = ry / 32;

    floor->addAsset(Vector3D(rx, ry, 0), "pics/test.tga", assetIndex);

    for (int i = 0; i < 3; ++i)
    {
        floor->addCollision(x,     y + 2 + i, false);
        floor->addCollision(x + 1, y + 2 + i, false);
    }

    floor->addRegion(Vector3D(x * 32, (y + 2) * 32 + regionOffsetY, 0), Vector3D(64,32,0));
    floor->addEntry(Vector3D(x * 32 + 32, (y + 2) * 32 + 4 + regionOffsetY, 0));

    if (destination)
    {
        floor->addChildRoom(destination, entryPoint, doorRegion);
    }
    else
    {
        generateRoom(floor, entryPoint, doorRegion, returnPoint, isLeft);
    }
}

void GameMapGraph::generateRoom(Room* floor,
                                unsigned entryPoint, 
                                unsigned doorRegion,
                                unsigned returnPoint,
                                bool isLeft)
{
    floor->addChildRoom("data/genericroom.xml", entryPoint, doorRegion);
    Room* genericRoom = floor->getChildRoom(floor->getChildRoomCount() - 1)->room;

    int itemArray[] = {5, 3, 8, 0, 6, 7, 10, 11, 4};
    genericRoom->addItem(Vector3D(250 + rand() % 100, 280 + rand() % 100, 0), itemArray[rand() % 9]);
    genericRoom->addItem(Vector3D(350 + rand() % 100, 280 + rand() % 100, 0), itemArray[rand() % 9]);
    genericRoom->addItem(Vector3D(200 + rand() % 100, 280 + rand() % 100, 0), itemArray[rand() % 9]);

    for (int i = 0; i < rand() % 5; ++i)
    {
        genericRoom->addEnemyPosition(Vector3D(200 + i * 32, 300 + rand() % 100, 0 ));
    }


    if (isLeft)
    {
        addDoorway(genericRoom, 33, 180, 0, returnPoint, 0, 50, floor, 6);
    }
    else
    {
        addDoorway(genericRoom, 500, 195, 0, returnPoint, 0, 30, floor, 7);
        //couch
        genericRoom->addAsset(Vector3D(30, 240, 0), "pics/test.tga", 0);
        genericRoom->addCollision(3, 240 / 32 + 2, true);
        genericRoom->addCollision(4, 240 / 32 + 2, true);
        genericRoom->addCollision(5, 240 / 32 + 1, true);
        genericRoom->addCollision(1, 240 / 32 + 3, true);
        genericRoom->addCollision(2, 240 / 32 + 3, true);
        genericRoom->addCollision(3, 240 / 32 + 3, true);
    }

}

void GameMapGraph::destroy()
{
    if (root)
    {
        printf("Destroying graph... root:%p\n", root);
        root->destroy(root);
        printf("deleting root...\n");
        delete root;
        root = nullptr;
        printf("Done.\n");
    }
}

