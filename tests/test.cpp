#include "../app/src/main/cpp/GameStuff/map/GameMapGraph.h"
#include <gtest/gtest.h>


class TestMapGraph : public ::testing::Test {
 protected: 

     ~TestMapGraph()
     {
         fdb.destroy();
     }

     void SetUp() override
     {
         fdb.load("../data/furniture.xml");
     }

     void EnterChildRooms(Room* room, Room* parentRoom);


     FurnitureDatabase fdb;
     DArray<Room*> visitedRooms;
};


TEST_F(TestMapGraph, roomDeletion)
{
    GameMapGraph gmg;

    gmg.generate(fdb);

    int roomCountBefore = Room::getExistingRooms();
    printf("Generated rooms: %d\n", roomCountBefore);

    gmg.destroy();

    int roomCountAfter = Room::getExistingRooms();


    ASSERT_TRUE(roomCountBefore > roomCountAfter);
    EXPECT_EQ(0, roomCountAfter);

}


void TestMapGraph::EnterChildRooms(Room* room, Room* parentRoom)
{
    visitedRooms.add(room);

    for (unsigned i = 0; i < room->getChildRoomCount(); ++i)
    {
        Room* chr = room->getChildRoom(i)->room;
        
        if (chr != parentRoom)
        {
            EnterChildRooms(chr, room);
        }
    }
}


TEST_F(TestMapGraph, roomPassability)
{
    GameMapGraph gmg;
    gmg.generate(fdb);
    visitedRooms.destroy();

    Room* r = gmg.getRoot();
    EnterChildRooms(r, nullptr);
   
    printf("Visited rooms: %lu\n", visitedRooms.count());
    printf("Generated rooms: %d\n", Room::getExistingRooms());
    ASSERT_TRUE(visitedRooms.count() == Room::getExistingRooms());
    visitedRooms.destroy();
    gmg.destroy();
}



int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
