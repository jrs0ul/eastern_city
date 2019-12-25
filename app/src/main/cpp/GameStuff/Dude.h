#ifndef DUDE_H
#define DUDE_H

#include "Actor.h"
#include "../Vectors.h"
#include "../TextureLoader.h"
#include "../FindPath.h"
#include "GameMap.h"
#include "CraftingRecipes.h"
#include "ItemInstance.h"

class Dude : public Actor
{
public:
    void      init(Vector3D& position);
    void      destroy();
    void      update(float deltaTime, unsigned char* Keys, GameMap& map, ItemDatabase& itemDb, Path& path);
    void      drawInventory(PicsContainer& pics, ItemDatabase& itemDb);
    void      useItem(unsigned index, ItemDatabase& itemDb);
    void      craftItem(Recipe* recipe, ItemDatabase& itemDb);
    bool      colidesWithRegion(GameMap& map, unsigned* regionIndex, unsigned* entryIndex);
    void      setPosition(Vector3D& position);
    void      setHealth(float newHealth){health = newHealth;}
    void      resetPathIndex(){pathIndex = 0;}
    unsigned  getItemCount(){return items.count();}
    ItemInstance* getItem(unsigned index);
    Vector3D* getPos(){return &pos;}
    int       getHealth(){return health;}
    int       getSatiation(){return satiation;}
private:

    bool isColiding(Vector3D newPos, GameMap& map);
    void addItemToInventory(ItemInstance* item, int inventorySlotIndex);
    int  findFreedInventorySlot();
    bool isNoMorePlaceInBag(int freedSlotIndex);

private:
    DArray<ItemInstance> items;
    int                  pathIndex;
    bool                 playWalkAnimation;
    bool                 walkAnimationDone;
    float                satiationProgress;
    float                freezingProgress;

    ItemInstance*        equipedBodyItem;

    int                  maxItems;
    int                  satiation;
};


#endif //DUDE_H
