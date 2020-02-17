#ifndef GAME_MAP_GRAPH_H
#define GAME_MAP_GRAPH_H

#include "../Polygon.h"
#include "Room.h"
#include "../FurnitureData.h"

struct RoomAndEntry;


struct iPos
{
    int x;
    int y;
    Room* room;
    bool visited;

    iPos(int nx, int ny)
    : x(nx)
    , y(ny)
    , room(nullptr)
    , visited(false)
    {}
};

class GameMapGraph
{
public:
    GameMapGraph()
    : root(nullptr)
    , furnitureDB(nullptr){}

    void init(FurnitureDatabase& furDB);

    void buildCity(Room* firstYard, Room* firstBuilding);
    void makeYard(Room* yard, Room* parent, int reachedFrom, iPos yardPos);
    bool isYardPositionTaken(iPos pos);
    void drawYardMap(float x, float y, PicsContainer& pics, Room* currentRoom);
    void findYardMapWidthHeight(int& w, int& h);

    Room* addFloors(Room* mainfloor, unsigned entranceIndex);

    void addTunnelLeft(Room* room);
    void addTunnelRight(Room* room);
    void addTunnelBottom(Room* room);
    void addTunnelFront(Room* room);
    void addFrontBuidingDoor(Room* room);
    void addLeftBuildingDoor(Room* room);
    void addRightBuildingDoor(Room* room);

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


    Room* root;
    FurnitureDatabase* furnitureDB;
    DArray<iPos> yardPositions;
    int firstPointX;
    int firstPointY;
};

#endif //GAME_MAP_GRAPH_H
