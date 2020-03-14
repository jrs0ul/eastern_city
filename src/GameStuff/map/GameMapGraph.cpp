#include "GameMapGraph.h"


static int yards = 0;

void GameMapGraph::generate(FurnitureDatabase& furDB)
{
    yards = 10;

    furnitureDB = &furDB;

    
    root = new Room();
    root->setMapName("data/genericroom.xml");
    root->addEntry(Vector3D(350, 278, 0));
    addDoorway(root, 25, 158, 0, 0, 0, 30, nullptr, 6, 78, 5, 0, false);

    root->addChildRoom("data/stairwell2.xml", 2, 0);
    root->addItem(Vector3D(300, 260, 0), rand() % 2);

    addCupboard(root);
    addWardrobe(root);
    addCouch(root, 110, 168);

    Room* stairwell = root->getChildRoom(0)->room;
    //exit outside
    
    addStairsOutside(stairwell);
    stairwell->addAsset(Vector3D(330, 5, 0), "pics/test.tga", 17);
    stairwell->addAsset(Vector3D(462, 104, 0), "pics/test.tga", 18);

    addStairsUp(stairwell);

    stairwell->addChildRoom("data/out.xml", 0, 0);

    addDoorway(stairwell, 672, 115, 2, 1, 2, 20, root, 4);


    addFloors(stairwell, 2);

    addDoorway(stairwell, 288, 98, 3, 0, 3, 0, nullptr, 2, 72, 5, 2);
    addDoorway(stairwell, 544, 96, 4, 0, 4, 0,  nullptr, 2, 72, 5, 2);

    buildCity(stairwell->getChildRoom(0)->room, stairwell);

    printf("done generating\n");
}

void GameMapGraph::buildCity(Room* firstYard, Room* firstBuilding)
{
    addFrontBuidingDoor(firstYard);
    firstYard->addChildRoom(firstBuilding, 0, 0);

    firstYard->addAsset(Vector3D(421, 132, 0), "pics/outside_sprites.tga", 2);//maroz

    yardPositions.destroy();
    makeYard(firstYard, nullptr, -1, iPos(0, 0));
}


void GameMapGraph::makeYard(Room* yard, Room* parent, int reachedFrom, iPos yardPos)
{
    --yards;
    //printf("YARD %d, located at (%d %d)\n", yards, yardPos.x, yardPos.y);
    yardPos.room = yard;
    yardPositions.add(yardPos);


    int entriesInThisYard = rand() % 2 + 1;

    if (entriesInThisYard > yards)
    {
        entriesInThisYard = yards;
    }

    bool canGoUp = !isYardPositionTaken(iPos(yardPos.x, yardPos.y - 1));
    bool canGoLeft = !isYardPositionTaken(iPos(yardPos.x - 1, yardPos.y));
    bool canGoRight = !isYardPositionTaken(iPos(yardPos.x + 1, yardPos.y));
    bool canGoDown = !isYardPositionTaken(iPos(yardPos.x, yardPos.y + 1));
    

    bool directions[4] = {canGoUp && reachedFrom != 3 && reachedFrom != -1,
                          canGoLeft && reachedFrom != 2,
                          canGoRight && reachedFrom != 1,
                          canGoDown && reachedFrom != 0};

    DArray<int> indexes;
    for (unsigned i = 0; i < 4; ++i)
    {
        if (directions[i] == 1)
        {
            indexes.add(i);
        }
    }

    if ((int)indexes.count() < entriesInThisYard)
    {
        entriesInThisYard = (int)indexes.count();
    }


    memset(directions, 0, sizeof(bool) * 4);

    for (int i = 0; i < entriesInThisYard; ++i)
    {
        int r = rand() % indexes.count();
        int randomIndex = indexes[r];
        directions[randomIndex] = 1;
        indexes.remove(r);
    }

    indexes.destroy();


    if (yards == 1 || yards == 0)
    {
        yard->addConstantEnemy(Vector3D(400 + 2 * 32, 400 + rand() % 100, 0 ), 3);
        printf("Adding a motherf*king BEAR!\n");
    }
    else
    {
        for (int i = 0; i < rand() % 15; ++i)
        {
            yard->addRespawningEnemy(Vector3D(400 + i * 32, 400 + rand() % 100, 0 ), 1);
        }
    }

    //FRONT
    if (reachedFrom != -1)
    {
        if (reachedFrom != 3)
        {
            if (directions[0] && !isYardPositionTaken(iPos(yardPos.x, yardPos.y - 1)))
            {
                yard->addChildRoom("data/out.xml", 3, 0);
                addTunnelFront(yard);
                makeYard(yard->getChildRoom(0)->room, yard, 0, iPos(yardPos.x, yardPos.y - 1));
            }
            else
            {
                addFrontBuidingDoor(yard);
                addBuilding(yard, 0);
            }

        }
        else
        {
            addTunnelFront(yard);
            yard->addChildRoom(parent, 3, 0);
        }
    }

    //LEFT
    if (reachedFrom != 2)
    {
        if (directions[1] && !isYardPositionTaken(iPos(yardPos.x - 1, yardPos.y)))
        {
            addTunnelLeft(yard);
            yard->addChildRoom("data/out.xml", 2, 1);

            makeYard(yard->getChildRoom(1)->room, yard, 1, iPos(yardPos.x - 1, yardPos.y));


        }
        else
        {
            addLeftBuildingDoor(yard);
            addBuilding(yard, 1);
        }
    }
    else
    {
        addTunnelLeft(yard);
        yard->addChildRoom(parent, 2, 1);
    }

    //RIGHT
    if (reachedFrom != 1)
    {
        if (directions[2] && !isYardPositionTaken(iPos(yardPos.x + 1, yardPos.y)))
        {
            addTunnelRight(yard);
            yard->addChildRoom("data/out.xml", 1, 2);
            makeYard(yard->getChildRoom(2)->room, yard, 2, iPos(yardPos.x + 1, yardPos.y));
        }
        else
        {
            addRightBuildingDoor(yard);
            addBuilding(yard,2);
        }

    }
    else
    {
        addTunnelRight(yard);
        yard->addChildRoom(parent, 1, 2);
    }

    //BOTTOM
    if (reachedFrom != 0)
    {
        if (directions[3] && !isYardPositionTaken(iPos(yardPos.x, yardPos.y + 1)))
        {
            addTunnelBottom(yard);
            yard->addChildRoom("data/out.xml", 0, 3);
            makeYard(yard->getChildRoom(3)->room, yard, 3, iPos(yardPos.x, yardPos.y + 1));
        }

    }
    else
    {
        addTunnelBottom(yard);
        yard->addChildRoom(parent, 0, 3);
    }

    if ((rand() % 4) % 2)
    {
        addDumpster(yard, 830 + rand() % 30, 220 + rand() % 50);
    }
    else
    {
        addBench(yard, 402, 197, false);
    }

    if (rand() % 2 == 0 && reachedFrom != 0)
    {
        addCar(yard);
    }

    int itemArray[] = {8, 23};
    yard->addItem(Vector3D(350 + rand() % 100, 348 + rand() % 100, 0), itemArray[rand() % 2]);
    yard->addItem(Vector3D(550 + rand() % 100, 348 + rand() % 100, 0), itemArray[rand() % 2]);
    yard->addItem(Vector3D(700 + rand() % 100, 448 + rand() % 100, 0), itemArray[rand() % 2]);


}

bool GameMapGraph::isYardPositionTaken(iPos pos)
{
    for (unsigned i = 0; i < yardPositions.count(); ++i)
    {
        if (yardPositions[i].x == pos.x && yardPositions[i].y == pos.y)
        {
            return true;
        }
    }

    return false;
}

void GameMapGraph::drawYardMap(float x, float y, PicsContainer& pics, Room* currentRoom)
{
    
    for (unsigned i = 0; i < yardPositions.count(); ++i)
    {
        if (!yardPositions[i].visited)
        {
            if (currentRoom == yardPositions[i].room)
            {
                yardPositions[i].visited = true;
            }

            continue;
        }

        COLOR roomColor = (currentRoom == yardPositions[i].room) ? COLOR(1,0,0, 0.6f) : COLOR(1,1,1,0.8f);
        pics.draw(-1, x + firstPointX * 16 + yardPositions[i].x * 16 ,
                      y + firstPointY * 16 + yardPositions[i].y * 16, 0, false, 16.f, 16.f, 0.f,
                  roomColor, roomColor);
    }
}

void GameMapGraph::findYardMapWidthHeight(int& w, int& h)
{
    int smallestX = 9999;
    int biggestX = -9999;
    int smallestY = 9999;
    int biggestY = -9999;
    
    for (unsigned i = 0; i < yardPositions.count(); ++i)
    {
        if (yardPositions[i].x > biggestX)
        {
            biggestX = yardPositions[i].x;
        }

        if (yardPositions[i].x < smallestX)
        {
            smallestX = yardPositions[i].x;
        }

        if (yardPositions[i].y > biggestY)
        {
            biggestY = yardPositions[i].y;
        }

        if (yardPositions[i].y < smallestY)
        {
            smallestY = yardPositions[i].y;
        }
    }

    w = biggestX - smallestX;
    h = biggestY - smallestY;

    firstPointX = yardPositions[0].x - smallestX;
    firstPointY = yardPositions[0].y - smallestY;

    printf("%d %d\n", firstPointX, firstPointY);
}

void GameMapGraph::addFloors(Room* mainfloor, unsigned entranceIndex)
{
    mainfloor->addChildRoom("data/stairwell2.xml", 0, 1);


    int additionalFloors = rand() % 3 + 1;
    //printf("BUILDING FLOORS: %d\n", additionalFloors + 2);

    Room* previousFloor = mainfloor;
    Room* currentFloor = mainfloor->getChildRoom(entranceIndex)->room;


    for (int i = 0; i < additionalFloors; ++i)
    {
        //printf("floor %d\n", 1 + i);

        if (previousFloor == mainfloor)
        {
            currentFloor->addChildRoom(previousFloor, 1, 0);
        }
        else
        {
            currentFloor->addChildRoom(previousFloor, 1, 0);
        }

        currentFloor->addAsset(Vector3D(462, 104, 0), "pics/test.tga", 18 + i + 1);

        currentFloor->addEntry(Vector3D(580, 160, 0));
        currentFloor->addRegion(Vector3D(560, 132,0), Vector3D(64, 64, 0));

        currentFloor->addChildRoom("data/stairwell2.xml", 0, 1);

        addStairsUp(currentFloor);
        
        currentFloor->addAsset(Vector3D(537, 0, 0), "pics/test.tga", 15); //stairs down
        currentFloor->addDoorHole(546, 638.9f, 21.f);

        addDoorway(currentFloor, 288, 98, 2, 0, 2, 0, nullptr, 2, 70, 5, 2);
        addApartamentDoorFront(298, 107, currentFloor);
        addDoorway(currentFloor, 672, 115, 3, 0, 3, 20, nullptr, 4);
        addDoorway(currentFloor, 40, 128, 4, 0, 4, 15, nullptr, 5, 65, 5, 1);

        previousFloor = currentFloor;
        currentFloor = currentFloor->getChildRoom(1)->room;
    }

    Room* lastFloor = currentFloor;
    lastFloor->addChildRoom(previousFloor, 1, 0);


    lastFloor->addEntry(Vector3D(580, 160, 0));
    lastFloor->addRegion(Vector3D(560, 132,0), Vector3D(64, 64, 0));
    lastFloor->addAsset(Vector3D(537, 0, 0), "pics/test.tga", 15); //stairs down
    lastFloor->addAsset(Vector3D(462, 104, 0), "pics/test.tga", 18 + additionalFloors + 1);
    lastFloor->addItem(Vector3D(360, 280, 0), 8);
    lastFloor->addItem(Vector3D(320, 250, 0), 8);
    lastFloor->addItem(Vector3D(360, 275, 0), 20);
    lastFloor->addDoorHole(546, 638.9f, 21.f);
    addDoorway(lastFloor, 288, 98, 1, 0, 1, 0, nullptr, 2, 64, 0, 2);
    addDoorway(lastFloor, 40, 128, 2, 0, 2, 50, nullptr, 5, 64, 1, 1);
    addDoorway(lastFloor, 672, 115, 3, 0, 3, 20, nullptr, 4);
}

void GameMapGraph::buildLastFloor(Room* floor)
{
    }

void GameMapGraph::addTunnelLeft(Room* room)
{
    SPolygon p;
    p.points.add(Vector3D(64, 666, 0));
    p.points.add(Vector3D(34, 418, 0));
    p.points.add(Vector3D(208, 432, 0));
    room->addVerticesAfter(p, 5);
    p.points.destroy();

    room->addAsset(Vector3D(25, 151, 0), "pics/outside_sprites.tga", 4);
    if (rand() % 2 == 0)
    {
        room->addAsset(Vector3D(85, 296, 0), "pics/outside_sprites.tga", 6);
    }
    room->addEntry(Vector3D(44.f, 425.f, 0.f));
    room->addRegion(Vector3D(10.f, 400.f, 0.f), Vector3D(64.f, 128.f, 0.f));
}

void GameMapGraph::addTunnelRight(Room* room)
{
    SPolygon p;
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

void GameMapGraph::addTunnelFront(Room* room)
{
    room->addEntry(Vector3D(700.f, 185.f, 0.f));
    room->addRegion(Vector3D(674.f, 160.f, 0.f), Vector3D(64.f, 64.f, 0.f));
    room->addDoorHole(629, 766, 35);
    room->addAsset(Vector3D(570, 13, 0), "pics/outside_sprites.tga", 7);
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
    SPolygon p;

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

void GameMapGraph::addRightBuildingDoor(Room* room)
{
    SPolygon p;

    p.points.add(Vector3D(1119, 370, 0));
    p.points.add(Vector3D(1042, 403, 0));
    p.points.add(Vector3D(1049, 417, 0));
    p.points.add(Vector3D(1153, 369, 0));
    p.points.add(Vector3D(1156, 352, 0));
    p.points.add(Vector3D(1214, 403, 0));
    p.points.add(Vector3D(1214, 413, 0));
    p.points.add(Vector3D(1272, 475, 0));
    p.points.add(Vector3D(1266, 496, 0));
    p.points.add(Vector3D(1179, 567, 0));
    p.points.add(Vector3D(1260, 577, 0));

    room->addVerticesAfter(p, 1);
    p.points.destroy();

    room->addAsset(Vector3D(1265, 200, 0), "pics/outside_sprites.tga", 1, true);
    room->addEntry(Vector3D(1183.f, 370.f, 0.f));
    room->addRegion(Vector3D(1170.f, 330.f, 0.f), Vector3D(64.f, 64.f, 0.f));
}


void GameMapGraph::addBuilding(Room* outside, unsigned regionIndex)
{
    outside->addChildRoom("data/stairwell2.xml", 0, regionIndex);

    Room* building = outside->getChildRoom(outside->getChildRoomCount() - 1)->room;
    building->addChildRoom(outside, regionIndex, 0);
   
    addStairsOutside(building);
    addStairsUp(building);
    building->addAsset(Vector3D(462, 104, 0), "pics/test.tga", 18);
    addFloors(building, 1);
    addDoorway(building, 672, 115, 2, 0, 2, 20, nullptr, 4);
}

void GameMapGraph::addStairsUp(Room* stairwell)
{
    stairwell->addAsset(Vector3D(0, 0, 0), "pics/test.tga", 3); //stairs
    stairwell->addDoorHole(140, 252, 100, 25);
    stairwell->addEntry(Vector3D(220, 75, 0));
    stairwell->addRegion(Vector3D(180, 70, 0), Vector3D(74, 48, 0));
}

void GameMapGraph::addStairsOutside(Room* stairwell)
{
    stairwell->addEntry(Vector3D(680, 280, 0));
    stairwell->addRegion(Vector3D(580, 330, 0), Vector3D(164, 64, 0));
    stairwell->addAsset(Vector3D(537, 288, 0), "pics/test.tga", 16);
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
                              int  doorLocation,
                              bool connectWithOtherRoom)
{

    floor->addAsset(Vector3D(rx, ry, 0), "pics/test.tga", assetIndex);

    floor->addDoorHole(rx + doorwayOffsetX, rx + doorwayOffsetX + doorWidth, 21);
   
    floor->addRegion(Vector3D(rx + doorwayOffsetX, ry + 64 + regionOffsetY, 0), Vector3D(64,32,0));
    floor->addEntry(Vector3D(rx + 32 + doorwayOffsetX, ry + 64 + 4 + regionOffsetY, 0));

    if (!connectWithOtherRoom)
    {
        return;
    }

    if (destination)
    {
        floor->addChildRoom(destination, entryPoint, doorRegion);
    }
    else
    {
        generateRoom(floor, entryPoint, doorRegion, returnPoint, doorLocation);
    }
}

void GameMapGraph::addBottomDoorway(Room* floor, unsigned returnPoint, Room* destination)
{
    floor->addAsset(Vector3D(260, 215, 0), "pics/test.tga", 14, false, true);
    floor->addRegion(Vector3D(260, 360, 0), Vector3D(100,16,0));
    floor->addEntry(Vector3D(320, 310, 0));
    floor->addChildRoom(destination, returnPoint, 0);
}

void GameMapGraph::generateRoom(Room* floor,
                                unsigned entryPoint, 
                                unsigned doorRegion,
                                unsigned returnPoint,
                                int doorLocation)
{
    floor->addChildRoom("data/genericroom.xml", entryPoint, doorRegion);
    Room* genericRoom = floor->getChildRoom(floor->getChildRoomCount() - 1)->room;

    if (rand() % 3 != 1)
    {//big carpet
        genericRoom->addAsset(Vector3D(66, 250, 0), "pics/test.tga", 12);
    }

    int itemArray[] = {5, 3, 8, 0, 6, 7, 10, 11, 4};
    genericRoom->addItem(Vector3D(250 + rand() % 100, 248 + rand() % 100, 0), itemArray[rand() % 9]);
    genericRoom->addItem(Vector3D(350 + rand() % 100, 248 + rand() % 100, 0), itemArray[rand() % 9]);
    genericRoom->addItem(Vector3D(200 + rand() % 100, 248 + rand() % 100, 0), itemArray[rand() % 9]);

    if (doorLocation != 1)
    {
        for (int i = 0; i < rand() % 4; ++i)
        {
            genericRoom->addConstantEnemy(Vector3D(200 + i * 32, 260 + rand() % 100, 0 ), 1);
        }
    }


    switch (doorLocation)
    {
        case 0 : addDoorway(genericRoom, 25, 158, 0, returnPoint, 0, 30, floor, 6, 78, 5); break; //left
        case 1 : addDoorway(genericRoom, 500, 163, 0, returnPoint, 0, 20, floor, 7, 56, -12); break; //right
        case 2 : addBottomDoorway(genericRoom, returnPoint, floor); //bottom
    }



    if ((rand() % 100) % 2 == 0)
    {
        addTvCupboard(genericRoom);
    }

    if (doorLocation != 1)
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

    if (doorLocation != 0)
    {
        addCouch(genericRoom, 20, 232);
    }

}

void GameMapGraph::addWardrobe(Room* room)
{

    FurnitureData* fd = furnitureDB->getFurniture(1);

    Furniture ward;

    ward.initFromData(fd);
       
    ward.pos = Vector3D(480, 148, 0);
    ward.collisionBodySize = Vector3D(78, 200, 0);
    ward.collisionPolygon.points.add(Vector3D(2, 169, 0));
    ward.collisionPolygon.points.add(Vector3D(42, 168, 0));
    ward.collisionPolygon.points.add(Vector3D(70, 185, 0));
    ward.collisionPolygon.points.add(Vector3D(25, 187, 0));
    ward.collisionPolygon.points.add(Vector3D(2, 169, 0));


    room->addFurniture(ward);

    ward.destroy();

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
    wardrobe.addItem(9);
   
    room->addItemContainer(1, wardrobe);

}

void GameMapGraph::addFridge(Room* room)
{
    Furniture fridge;

    fridge.initFromData(furnitureDB->getFurniture(2));

    fridge.pos = Vector3D(472, 203, 0);
    fridge.collisionBodySize = Vector3D(108, 146, 0);
    fridge.collisionPolygon.points.add(Vector3D(5, 90, 0));
    fridge.collisionPolygon.points.add(Vector3D(45, 90, 0));
    fridge.collisionPolygon.points.add(Vector3D(70, 120, 0));
    fridge.collisionPolygon.points.add(Vector3D(30, 120, 0));
    fridge.collisionPolygon.points.add(Vector3D(5, 90, 0));

    room->addFurniture(fridge);

    fridge.destroy();

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
    tvCupboard.initFromData(furnitureDB->getFurniture(3));
    tvCupboard.pos = Vector3D(135, 168, 0);
    tvCupboard.collisionBodySize = Vector3D(101, 118, 0);
    tvCupboard.collisionPolygon.points.add(Vector3D(60, 73, 0));
    tvCupboard.collisionPolygon.points.add(Vector3D(94, 75, 0));
    tvCupboard.collisionPolygon.points.add(Vector3D(42, 104, 0));
    tvCupboard.collisionPolygon.points.add(Vector3D(4, 96, 0));
    tvCupboard.collisionPolygon.points.add(Vector3D(60, 73, 0));

    room->addFurniture(tvCupboard);

    tvCupboard.destroy();

    ItemContainer container;
    container.init(3, 1);

    int junkstuff[] = {4, 8, 5, 22, 13};

    for (int i = 0; i < 2; ++i)
    {
        container.addItem(junkstuff[rand() % 5]);
    }

    room->addItemContainer(room->getFurnitureCount() - 1, container);
}

void GameMapGraph::addCupboard(Room* room)
{
    Furniture cupboard;
    cupboard.initFromData(furnitureDB->getFurniture(4));
    cupboard.pos = Vector3D(445, 203, 0);
    cupboard.collisionBodySize = Vector3D(92, 118, 0);
    cupboard.collisionPolygon.points.add(Vector3D(1, 62, 0));
    cupboard.collisionPolygon.points.add(Vector3D(44, 62, 0));
    cupboard.collisionPolygon.points.add(Vector3D(76, 103, 0));
    cupboard.collisionPolygon.points.add(Vector3D(34, 107, 0));
    cupboard.collisionPolygon.points.add(Vector3D(1, 62, 0));

    room->addFurniture(cupboard);

    cupboard.destroy();

    ItemContainer container2;
    container2.init(4, 2);
    
    int junkstuff[] = {3, 7, 13, 8, 22};

    for (int i = 0; i < 3; ++i)
    {
        container2.addItem(junkstuff[rand() % 5]);
    }

    room->addItemContainer(room->getFurnitureCount() - 1, container2);
}

void GameMapGraph::addCouch(Room* room, int x, int y)
{
    Furniture couch;
    couch.initFromData(furnitureDB->getFurniture(0));
    couch.pos = Vector3D(x, y, 0);
    couch.isBed = true;
    couch.collisionBodySize = Vector3D(150, 126, 0);

    couch.collisionPolygon.points.add(Vector3D(80, 60, 0));
    couch.collisionPolygon.points.add(Vector3D(140, 60, 0));
    couch.collisionPolygon.points.add(Vector3D(120, 85, 0));
    couch.collisionPolygon.points.add(Vector3D(70, 105, 0));
    couch.collisionPolygon.points.add(Vector3D(5, 95, 0));
    couch.collisionPolygon.points.add(Vector3D(80, 60, 0));
    room->addFurniture(couch);

    couch.destroy();
}

void GameMapGraph::addDumpster(Room* room, int x, int y)
{
    Furniture dumpster;
    dumpster.initFromData(furnitureDB->getFurniture(5));
    dumpster.pos = Vector3D(x, y, 0);
    dumpster.collisionBodySize = Vector3D(159, 124, 0);

    dumpster.collisionPolygon.points.add(Vector3D((63 - 54),(329 - 248), 0));
    dumpster.collisionPolygon.points.add(Vector3D((110 - 54), (323 - 248), 0));
    dumpster.collisionPolygon.points.add(Vector3D((169 - 54), (325 - 248), 0));
    dumpster.collisionPolygon.points.add(Vector3D((184 - 54), (355 - 248), 0));
    dumpster.collisionPolygon.points.add(Vector3D((78 - 54), (355 - 248), 0));
    dumpster.collisionPolygon.points.add(Vector3D((63 - 54),(329 - 248), 0));

    room->addFurniture(dumpster);

    dumpster.destroy();

    ItemContainer container;
    container.init(3, 3);
    
    int junkstuff[] = {1, 4, 8, 20};

    for (int i = 0; i < 3; ++i)
    {
        container.addItem(junkstuff[rand() % 4]);
    }

    room->addItemContainer(room->getFurnitureCount() - 1, container);
}

void GameMapGraph::addBench(Room* room, int x, int y, bool flipped)
{
    Furniture bench;

    bench.initFromData(furnitureDB->getFurniture(6));
    bench.pos = Vector3D(x, y, 0);
    bench.collisionBodySize = Vector3D(145, 91, 0);

    bench.collisionPolygon.points.add(Vector3D(12, 56, 0));
    bench.collisionPolygon.points.add(Vector3D(75, 51, 0));
    bench.collisionPolygon.points.add(Vector3D(136, 59, 0));
    bench.collisionPolygon.points.add(Vector3D(125, 84, 0));
    bench.collisionPolygon.points.add(Vector3D(125, 84, 0));
    bench.collisionPolygon.points.add(Vector3D(1, 84, 0));
    bench.collisionPolygon.points.add(Vector3D(12, 56, 0));

    room->addFurniture(bench);
    bench.destroy();
}

void GameMapGraph::addCar(Room* room)
{
    Furniture car;
    car.pos = Vector3D(313, 552, 0);
    car.pictureIndex = 21;
    car.spriteIndex = 10;
    car.furnitureDbIndex = 666;
    car.collisionBodySize = Vector3D(382, 171, 0);
    
    
    car.collisionPolygon.points.add(Vector3D(33, 80, 0));
    car.collisionPolygon.points.add(Vector3D(198, 72, 0));
    car.collisionPolygon.points.add(Vector3D(384, 84, 0));
    car.collisionPolygon.points.add(Vector3D(391, 121, 0));
    car.collisionPolygon.points.add(Vector3D(368, 138, 0));
    car.collisionPolygon.points.add(Vector3D(242, 147, 0));
    car.collisionPolygon.points.add(Vector3D(122, 141, 0));
    car.collisionPolygon.points.add(Vector3D(125, 168, 0));
    car.collisionPolygon.points.add(Vector3D(106, 186, 0));
    car.collisionPolygon.points.add(Vector3D(82, 180, 0));
    car.collisionPolygon.points.add(Vector3D(72, 168, 0));
    car.collisionPolygon.points.add(Vector3D(73, 148, 0));
    car.collisionPolygon.points.add(Vector3D(18, 133, 0));
    
    car.collisionPolygon.points.add(Vector3D(33, 80, 0));

    room->addFurniture(car);
    car.destroy();
}

void GameMapGraph::addApartamentDoorFront(int x, int y, Room* room)
{
    Furniture doorFront;
    doorFront.initFromData(furnitureDB->getFurniture(7));
    doorFront.pos = Vector3D(x, y, 0);
    doorFront.collisionBodySize = Vector3D(58, 110, 0);
    doorFront.collisionPolygon.points.add(Vector3D(0, 0, 0));
    doorFront.collisionPolygon.points.add(Vector3D(57, 0, 0));
    doorFront.collisionPolygon.points.add(Vector3D(57, 90, 0));
    doorFront.collisionPolygon.points.add(Vector3D(0, 90, 0));
    doorFront.collisionPolygon.points.add(Vector3D(0, 0, 0));
    room->addFurniture(doorFront);
    doorFront.destroy();
    
}

void GameMapGraph::destroy()
{
    yardPositions.destroy();

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

