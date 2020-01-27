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

void Room::addItem(Vector3D pos, ItemInstance* item)
{
    ItemInstance newItem = *item;
    newItem.setPosition(pos);
    items.add(newItem);
}

void Room::addItemContainer(unsigned furnitureIndex, ItemContainer& container)
{
    if (furnitureIndex > furniture.count())
    {
        return;
    }

    furniture[furnitureIndex].itemContainerIndex = itemContainers.count();
    itemContainers.add(container);
}

void Room::addEnemyPosition(Vector3D pos)
{
    enemies.add(pos);
}

void Room::addAsset(Vector3D pos, 
                    const char* name, 
                    unsigned spriteIndex, 
                    bool flipped,
                    bool frontLayer)
{
    Asset ass;
    ass.pos = pos;
    ass.isFlipped = flipped;
    ass.isInFrontLayer = frontLayer;
    strcpy(ass.name, name);
    ass.spriteIndex = spriteIndex;
    assets.add(ass);
}

void Room::addFurniture(Furniture* f)
{
    Furniture fur = *f;
    furniture.add(fur);
}

void Room::addCollisionPolygon(Polygon& p)
{
    Polygon pol;
    collisionPolygons.add(pol);
    Polygon* pp = &collisionPolygons[collisionPolygons.count() - 1];

    for (unsigned i = 0; i < p.points.count(); ++i)
    {
        pp->points.add(p.points[i]);
    }

}

void Room::addDoorHole(float x1, float x2, float height)
{
    doorHoles.add(Vector3D(x1, x2, height));
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

void  Room::addVerticesAfter(Polygon& p, unsigned indexToPutVerticesAfter)
{
    AdditionalVertices av;

    for (unsigned i = 0; i < p.points.count(); ++i)
    {
        av.p.points.add(p.points[i]);
    }

    av.index = indexToPutVerticesAfter;

    additionalVertices.add(av);
                
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

unsigned Room::getItemContainerCount()
{
    return itemContainers.count();
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

unsigned Room::getFurnitureCount()
{
    return furniture.count();
}

unsigned Room::getAdditionalRegionsCount()
{
    return additionalRegions.count();
}

unsigned Room::getAdditionalEntriesCount()
{
    return additionalEntries.count();
}

unsigned Room::getCollisionPolygonCount()
{
    return collisionPolygons.count();
}

unsigned Room::getDoorHoleCount()
{
    return doorHoles.count();
}

ItemInstance* Room::getItem(unsigned index)
{
    if (index < items.count())
    {
        return &items[index];
    }

    return nullptr;
}

ItemContainer* Room::getItemContainer(unsigned index)
{
    if (index < itemContainers.count())
    {
        return &itemContainers[index];
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

Furniture* Room::getFurniture(unsigned index)
{
    if (index < furniture.count())
    {
        return &furniture[index];
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

Polygon* Room::getCollisionPolygon(unsigned index)
{
    if (index < collisionPolygons.count())
    {
        return &collisionPolygons[index];
    }

    return nullptr;

}

AdditionalVertices* Room::getAdditionalVertices(unsigned index)
{
    if (index < additionalVertices.count())
    {
        return &additionalVertices[index];
    }

    return nullptr;
}

Vector3D* Room::getDoorHole(unsigned index)
{
    if (index < doorHoles.count())
    {
        return &doorHoles[index];
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

    for (unsigned i = 0; i < itemContainers.count(); ++i)
    {
        itemContainers[i].destroy();
    }

    for (unsigned i = 0; i < additionalVertices.count(); ++i)
    {
        additionalVertices[i].p.points.destroy();
    }

    additionalVertices.destroy();

    itemContainers.destroy();

    childRooms.destroy();
    items.destroy();
    enemies.destroy();
    assets.destroy();
    doorHoles.destroy();

    for (unsigned i = 0; i < collisionPolygons.count(); ++i)
    {
        collisionPolygons[i].points.destroy();
    }

    collisionPolygons.destroy();
    
    for (unsigned i = 0; i < furniture.count(); ++i)
    {
        furniture[i].destroy();
    }

    furniture.destroy();


    additionalRegions.destroy();
    additionalEntries.destroy();
}



void GameMapGraph::init()
{
    root = new Room();
    root->setMapName("data/room.xml");

    root->addChildRoom("data/stairwell.xml", 2, 0);
    root->addItem(Vector3D(300, 260, 0), rand() % 2);

    addCupboard(root);
    //---
    Furniture ward;
    ward.pictureIndex = 5;
    ward.spriteIndex = 1;
    ward.pos = Vector3D(480, 180, 0);
    ward.collisionBodySize = Vector3D(78, 200, 0);
    ward.collisionPolygon.points.add(Vector3D(2, 169, 0));
    ward.collisionPolygon.points.add(Vector3D(42, 168, 0));
    ward.collisionPolygon.points.add(Vector3D(70, 185, 0));
    ward.collisionPolygon.points.add(Vector3D(25, 187, 0));
    ward.collisionPolygon.points.add(Vector3D(2, 169, 0));


    root->addFurniture(&ward);

    ItemContainer wardrobe;
    wardrobe.init(10, 3);

    int itemArray[] = {5, 3, 8, 0, 6, 7, 10, 11, 4};


    wardrobe.addItem(itemArray[rand() % 9]);
    wardrobe.addItem(itemArray[rand() % 9]);
    wardrobe.addItem(itemArray[rand() % 9]);
    wardrobe.addItem(12);
    wardrobe.addItem(13);
    wardrobe.addItem(2);
    wardrobe.addItem(19);
   
    root->addItemContainer(1, wardrobe);
    //---

    addCouch(root, 110, 200);
    

    Room* stairwell = root->getChildRoom(0)->room;
    stairwell->addChildRoom("data/out.xml", 0, 0);
    
    addDoorway(stairwell, 672, 120, 2, 1, 2, 19, root, 4);

    Room* lastFloor = addFloors(stairwell, 2);

    lastFloor->addDoorHole(546, 638.9f, 21.f);
   
    addDoorway(lastFloor, 288, 98, 2, 0, 2, 0, nullptr, 2, 72, 3);
    lastFloor->addItem(Vector3D(300, 300, 0), 8);
    lastFloor->addItem(Vector3D(360, 320, 0), 8);
    lastFloor->addItem(Vector3D(320, 340, 0), 8);
    lastFloor->addItem(Vector3D(360, 305, 0), 8);
    Polygon poly;
    poly.points.add(Vector3D(144, 50, 0));
    poly.points.add(Vector3D(261, 50, 0));
    poly.points.add(Vector3D(262, 205, 0));
    poly.points.add(Vector3D(141, 204, 0));
    poly.points.add(Vector3D(144, 50, 0));
    lastFloor->addCollisionPolygon(poly);
    addDoorway(stairwell, 288, 98, 3, 0, 3, 0, nullptr, 2, 72, 5);
    addDoorway(stairwell, 544, 96, 4, 0, 4, 0,  nullptr, 2, 72, 5);

    //---------
    Room* outside = stairwell->getChildRoom(0)->room;
    
    addFrontBuidingDoor(outside);
    outside->addChildRoom(stairwell, 0, 0);

    outside->addAsset(Vector3D(421, 132, 0), "pics/outside_sprites.tga", 2);//maroz

    addTunnelLeft(outside);
    addTunnelRight(outside);
    addTunnelBottom(outside);
    //-----------
    //LEFT
    outside->addChildRoom("data/out.xml", 2, 1);
    
    Room* outsideLeft = outside->getChildRoom(1)->room;
    addFrontBuidingDoor(outsideLeft);
    addLeftBuildingDoor(outsideLeft);
    addTunnelRight(outsideLeft);
    addBuilding(outsideLeft, 1);
    addBuilding(outsideLeft, 0);

    //----------

    outside->addChildRoom("data/outsideR.xml", 1, 2);
    outside->addChildRoom("data/outsideB.xml", 0, 3);

    Room* outsideRight = outside->getChildRoom(2)->room;
    Room* outsideBottom = outside->getChildRoom(3)->room;

    outsideLeft->addChildRoom(outside, 1, 2);
    outsideRight->addChildRoom(outside, 2, 1);
    outsideBottom->addChildRoom(outside, 3, 0);

    outsideRight->addDoorHole(669, 736, 15);
    outsideBottom->addDoorHole(629, 766, 35);


    addBuilding(outsideRight, 0);
    addBuilding(outsideRight, 2);

    for (int i = 0; i < 15; ++i)
    {
        outside->addEnemyPosition(Vector3D(400 + i * 32, 400 + rand() % 100, 0 ));
    }

    poly.points.destroy();
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


        currentFloor->addDoorHole(546, 638.9f, 21.f);
        addDoorway(currentFloor, 288, 98, 2, 0, 2, 0, nullptr, 2, 70, 5);
        addDoorway(currentFloor, 672, 115, 3, 0, 3, 20, nullptr, 4);
        addDoorway(currentFloor, 40, 128, 4, 0, 4, 15, nullptr, 5, 65, 5, false);


        previousFloor = currentFloor;
        currentFloor = currentFloor->getChildRoom(1)->room;

    }

    Room* lastFloor = currentFloor;
    lastFloor->addChildRoom(previousFloor, 1, 0);

    return lastFloor;
}

void GameMapGraph::addTunnelLeft(Room* room)
{
    Polygon p;
    p.points.add(Vector3D(64, 666, 0));
    p.points.add(Vector3D(34, 418, 0));
    p.points.add(Vector3D(208, 432, 0));
    room->addVerticesAfter(p, 5);
    p.points.destroy();

    room->addAsset(Vector3D(25, 151, 0), "pics/outside_sprites.tga", 4);
    room->addEntry(Vector3D(44.f, 425.f, 0.f));
    room->addRegion(Vector3D(10.f, 400.f, 0.f), Vector3D(64.f, 128.f, 0.f));
}

void GameMapGraph::addTunnelRight(Room* room)
{
    Polygon p;
    p.points.add(Vector3D(1167, 427, 0));
    p.points.add(Vector3D(1340, 416, 0));
    p.points.add(Vector3D(1307, 666, 0));
    room->addVerticesAfter(p, 1);
    p.points.destroy();

    room->addAsset(Vector3D(1288, 151, 0), "pics/outside_sprites.tga", 4, true);
    room->addEntry(Vector3D(1326.f, 439.f, 0.f));
    room->addRegion(Vector3D(1315.f, 400.f, 0.f), Vector3D(64.f, 128.f, 0.f));
}

void GameMapGraph::addTunnelBottom(Room* room)
{
    room->addEntry(Vector3D(688.f, 700.f, 0.f));
    room->addRegion(Vector3D(480.f, 765.f, 0.f), Vector3D(370.f, 40.f, 0.f));
    room->addAsset(Vector3D(440, 470, 0), "pics/outside_sprites.tga", 5, false, true);
    room->addAsset(Vector3D(855, 470, 0), "pics/outside_sprites.tga", 5, true, true);
}

void GameMapGraph::addFrontBuidingDoor(Room* room)
{
    room->addEntry(Vector3D(700.f, 185.f, 0.f));
    room->addRegion(Vector3D(674.f, 160.f, 0.f), Vector3D(64.f, 64.f, 0.f));
    room->addDoorHole(669, 736, 15);
    room->addAsset(Vector3D(570, 53, 0), "pics/outside_sprites.tga", 0);
}

void GameMapGraph::addLeftBuildingDoor(Room* room)
{
    Polygon p;

    p.points.add(Vector3D(89, 610, 0));
    p.points.add(Vector3D(168, 597, 0));
    p.points.add(Vector3D(80, 526, 0));
    p.points.add(Vector3D(78, 510, 0));
    p.points.add(Vector3D(136, 448, 0));
    p.points.add(Vector3D(132, 438, 0));
    p.points.add(Vector3D(190, 377, 0));
    p.points.add(Vector3D(192, 394, 0));
    p.points.add(Vector3D(300, 442, 0));
    p.points.add(Vector3D(304, 428, 0));
    p.points.add(Vector3D(236, 395, 0));

    room->addVerticesAfter(p, 5);
    p.points.destroy();

    room->addAsset(Vector3D(25, 225, 0), "pics/outside_sprites.tga", 1);
    room->addEntry(Vector3D(170.f, 410.f, 0.f));
    room->addRegion(Vector3D(120.f, 360.f, 0.f), Vector3D(64.f, 64.f, 0.f));
}

void GameMapGraph::addBuilding(Room* outside, unsigned regionIndex)
{
    outside->addChildRoom("data/stairwell.xml", 0, regionIndex);
    Room* building = outside->getChildRoom(outside->getChildRoomCount() - 1)->room;
    building->addChildRoom(outside, regionIndex, 0);
    
    Room* lastFloor = addFloors(building, 1);
    Polygon poly;
    poly.points.add(Vector3D(144, 50, 0));
    poly.points.add(Vector3D(261, 50, 0));
    poly.points.add(Vector3D(262, 205, 0));
    poly.points.add(Vector3D(141, 204, 0));
    poly.points.add(Vector3D(144, 50, 0));

    lastFloor->addCollisionPolygon(poly);
    lastFloor->addDoorHole(546, 638.9f, 21.f);
    addDoorway(lastFloor, 288, 98, 2, 0, 2, 0, nullptr, 2);
    addDoorway(lastFloor, 40, 128, 3, 0, 3, 50, nullptr, 5, 64, 0, false);
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
                              float doorWidth,
                              float doorwayOffsetX,
                              bool isLeft)
{

    floor->addAsset(Vector3D(rx, ry, 0), "pics/test.tga", assetIndex);

    floor->addDoorHole(rx + doorwayOffsetX, rx + doorwayOffsetX + doorWidth, 21);
   
    floor->addRegion(Vector3D(rx + doorwayOffsetX, ry + 64 + regionOffsetY, 0), Vector3D(64,32,0));
    floor->addEntry(Vector3D(rx + 32 + doorwayOffsetX, ry + 64 + 4 + regionOffsetY, 0));

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

    if (isLeft)
    {
        for (int i = 0; i < rand() % 4; ++i)
        {
            genericRoom->addEnemyPosition(Vector3D(200 + i * 32, 300 + rand() % 100, 0 ));
        }
    }


    if (isLeft)
    {
        addDoorway(genericRoom, 25, 190, 0, returnPoint, 0, 30, floor, 6, 78, 5);
    }
    else
    {
        addDoorway(genericRoom, 500, 195, 0, returnPoint, 0, 20, floor, 7, 56, -12);
    }

    if ((rand() % 100) % 2 == 0)
    {
        addTvCupboard(genericRoom);
    }

    if (isLeft)
    {
        if ((rand() % 100) < 50)
        {
            addFridge(genericRoom);
        }
        else
        {
            addCupboard(genericRoom);
        }
    }

    if (!isLeft)
    {
        addCouch(genericRoom, 20, 264);
    }

}

void GameMapGraph::addFridge(Room* room)
{
    Furniture fridge;
    fridge.pos = Vector3D(472, 235, 0);
    fridge.collisionBodySize = Vector3D(108, 146, 0);
    fridge.spriteIndex = 9;
    fridge.pictureIndex = 5;
    fridge.collisionPolygon.points.add(Vector3D(5, 90, 0));
    fridge.collisionPolygon.points.add(Vector3D(45, 90, 0));
    fridge.collisionPolygon.points.add(Vector3D(70, 120, 0));
    fridge.collisionPolygon.points.add(Vector3D(30, 120, 0));
    fridge.collisionPolygon.points.add(Vector3D(5, 90, 0));

    room->addFurniture(&fridge);
    ItemContainer container;
    container.init(6, 3);
    int foods[] = {1, 0, 10, 14, 15};

    for (int i = 0; i < 4; ++i)
    {
        container.addItem(foods[rand() % 5]);
    }

    room->addItemContainer(room->getFurnitureCount() - 1, container);
}

void GameMapGraph::addTvCupboard(Room* room)
{
    Furniture tvCupboard;
    tvCupboard.pos = Vector3D(135, 200, 0);
    tvCupboard.collisionBodySize = Vector3D(101, 118, 0);
    tvCupboard.pictureIndex = 5;
    tvCupboard.spriteIndex = 10;
    tvCupboard.collisionPolygon.points.add(Vector3D(60, 73, 0));
    tvCupboard.collisionPolygon.points.add(Vector3D(94, 75, 0));
    tvCupboard.collisionPolygon.points.add(Vector3D(42, 104, 0));
    tvCupboard.collisionPolygon.points.add(Vector3D(4, 96, 0));
    tvCupboard.collisionPolygon.points.add(Vector3D(60, 73, 0));

    room->addFurniture(&tvCupboard);
    ItemContainer container;
    container.init(3, 1);

    int junkstuff[] = {4, 8, 5, 13};

    for (int i = 0; i < 2; ++i)
    {
        container.addItem(junkstuff[rand() % 4]);
    }

    room->addItemContainer(room->getFurnitureCount() - 1, container);
}

void GameMapGraph::addCupboard(Room* room)
{
    Furniture cupboard;
    cupboard.pos = Vector3D(445, 235, 0);
    cupboard.pictureIndex = 5;
    cupboard.spriteIndex = 11;
    cupboard.collisionBodySize = Vector3D(92, 118, 0);
    cupboard.collisionPolygon.points.add(Vector3D(1, 62, 0));
    cupboard.collisionPolygon.points.add(Vector3D(44, 62, 0));
    cupboard.collisionPolygon.points.add(Vector3D(76, 103, 0));
    cupboard.collisionPolygon.points.add(Vector3D(34, 107, 0));
    cupboard.collisionPolygon.points.add(Vector3D(1, 62, 0));

    room->addFurniture(&cupboard);
    ItemContainer container2;
    container2.init(4, 2);
    
    int junkstuff[] = {3, 7, 13, 8};

    for (int i = 0; i < 3; ++i)
    {
        container2.addItem(junkstuff[rand() % 4]);
    }

    room->addItemContainer(room->getFurnitureCount() - 1, container2);
}

void GameMapGraph::addCouch(Room* room, int x, int y)
{
    Furniture couch;
    couch.pos = Vector3D(x, y, 0);
    couch.pictureIndex = 5;
    couch.spriteIndex = 0;
    couch.isBed = true;
    couch.collisionBodySize = Vector3D(150, 126, 0);

    couch.collisionPolygon.points.add(Vector3D(80, 60, 0));
    couch.collisionPolygon.points.add(Vector3D(140, 60, 0));
    couch.collisionPolygon.points.add(Vector3D(120, 85, 0));
    couch.collisionPolygon.points.add(Vector3D(70, 105, 0));
    couch.collisionPolygon.points.add(Vector3D(5, 95, 0));
    couch.collisionPolygon.points.add(Vector3D(80, 60, 0));
    room->addFurniture(&couch);
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

