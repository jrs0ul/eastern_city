#ifndef GAME_MAP_GRAPH_H
#define GAME_MAP_GRAPH_H

#include "../Polygon.h"

struct RoomAndEntry;
class Room;
class FurnitureDatabase;
class PicsContainer;

enum EntranceEnum
{
    FRONT_ENTRANCE = 0,
    LEFT_ENTRANCE,
    RIGHT_ENTRANCE,
    BOTTOM_ENTRANCE,
    COUNT_ENTRANCE
};

struct iPos
{
    int x;
    int y;
    Room* room;
    Room* buildingEntrances[COUNT_ENTRANCE];
    Room* yardEntrances[COUNT_ENTRANCE];
    bool visitedBuildingEntrances[COUNT_ENTRANCE];
    bool visited;

    iPos(int nx, int ny)
    : x(nx)
    , y(ny)
    , room(nullptr)
    , visited(false)
    {
        for (int i = 0; i < COUNT_ENTRANCE; ++i)
        {
            buildingEntrances[i] = nullptr;
            visitedBuildingEntrances[i] = false;
            yardEntrances[i] = nullptr;
        }
    }
};

class GameMapGraph
{
public:
    GameMapGraph()
    : root(nullptr)
    , furnitureDB(nullptr){}

    void generate(FurnitureDatabase& furDB);

    void buildCity(Room* firstYard, Room* firstBuilding);
    void makeYard(Room* yard, Room* parent, int reachedFrom, iPos yardPos);
    bool isYardPositionTaken(iPos pos);
    void drawYardMap(float x, float y, PicsContainer& pics, Room* currentRoom);
    void findYardMapWidthHeight(int& w, int& h);

    void addFloors(Room* mainfloor, unsigned entranceIndex);
    void buildLastFloor(Room* floor);

    void addTunnelLeft(Room* room);
    void addTunnelRight(Room* room);
    void addTunnelBottom(Room* room);
    void addTunnelFront(Room* room);
    void addFrontBuidingDoor(Room* room);
    void addLeftBuildingDoor(Room* room);
    void addRightBuildingDoor(Room* room);

    Room* addBuilding(Room* outside, unsigned regionIndex);

    void addStairsUp(Room* stairwell);
    void addStairsOutside(Room* stairwell);

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
                    int doorLocation = 0,
                    bool connectWithOtherRoom = true);

    void addBottomDoorway(Room* floor, unsigned returnPoint, Room* destination);
    
    void generateRoom(Room* floor, 
                      unsigned entryPoint, 
                      unsigned doorRegion,
                      unsigned returnPoint,
                      int isLeft = 0);

    void addWardrobe(Room* room);
    void addFridge(Room* room);
    void addTvCupboard(Room* room);
    void addCupboard(Room* room);
    void addCouch(Room* room, int x, int y);

    void addDumpster(Room* room, int x, int y);
    void addBench(Room* room, int x, int y, bool flipped);
    void addCar(Room* room);
    void addApartamentDoorFront(int x, int y, Room* room);

    void destroy();

    Room* getRoot(){return root;}

private:

    Room* root;
    FurnitureDatabase* furnitureDB;
    DArray<iPos> yardPositions;
    int firstPointX;
    int firstPointY;

    int roomCount;
};

#endif //GAME_MAP_GRAPH_H
