#include "Statistics.h"
#include <cstring>
#include <cstdio>


void Statistics::init(const char* versionString, unsigned currentPlatform)
{
    strcpy(version, versionString);
    platform = currentPlatform;
    timesitemsAddedToInventory = 0;
    reset();
}


void Statistics::increaseItemAddition()
{
    ++timesitemsAddedToInventory;
}

void Statistics::increaseVisitedRooms()
{
    ++roomsVisited;
}

void Statistics::increaseCraftedItems()
{
    ++timesitemsCrafted;
}

void Statistics::increaseEquipedTimes()
{
    ++timesItemsEquiped;
}

void Statistics::increaseItemUsage()
{
    ++timesItemsUsed;
}

void Statistics::reset()
{
    timesitemsAddedToInventory = 0;
    roomsVisited = 0;
    timesItemsEquiped = 0;
    timesitemsCrafted = 0;
    timesItemsUsed = 0;
}

void Statistics::send(float gameTime, time_t realTime, bool isExit, char* output)
{
    printf("STATS: [version: %s, platform: %u]\n", version, platform);
    printf("STATS: [game time: %f, real time seconds: %ld]\n", gameTime, realTime);
    printf("STATS: [picked up items: %u, times items equiped: %u, crafted items: %u, items used: %u]\n",
           timesitemsAddedToInventory,
           timesItemsEquiped,
           timesitemsCrafted,
           timesItemsUsed);
    printf("STATS: [visited rooms: %u]\n", roomsVisited);

    sprintf(output, "key=super_slav&version=%s&platform=%u&exited=%d&gameTime=%d&realTime=%ld&pickedUp=%d&equiped=%d&crafted=%d&used=%d&visitedRooms=%d", 
            version, platform, isExit, (int)gameTime, realTime, timesitemsAddedToInventory, timesItemsEquiped, timesitemsCrafted,
            timesItemsUsed, roomsVisited);
}



