#ifndef STATISTICS_H
#define STATISTICS_H

#include <ctime>

class Statistics
{
public:

    static Statistics* getInstance()
    {
        static Statistics instance;
        return &instance;
    }
    void init(const char* versionString, unsigned currentPlatform);
    
    void increaseItemAddition();
    void increaseVisitedRooms();
    void increaseCraftedItems();
    void increaseEquipedTimes();
    void increaseItemUsage();

    void reset();

    void send(float gameTime, time_t realTime, bool isExit, char* output);
private:
    Statistics(){};
    ~Statistics(){};
private:

    char          version[128];
    unsigned      platform;
    unsigned      timesitemsAddedToInventory;
    unsigned      timesitemsCrafted;
    unsigned      roomsVisited;
    unsigned      timesItemsEquiped;
    unsigned      timesItemsUsed;

};


#endif //STATISTICS_H
