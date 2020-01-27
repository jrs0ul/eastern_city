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
    void          init(Vector3D& position);
    void          destroy();
    void          update(float deltaTime, 
                         unsigned char* Keys, 
                         GameMap& map,
                         float darkness,
                         ItemDatabase& itemDb, 
                         FindPath& path);

    void          drawInventory(PicsContainer& pics, ItemDatabase& itemDb, ItemInstance* selectedItem);
    int           hasItem(unsigned itemId);
    void          removeItem(unsigned index);
    void          useItem(ItemInstance*, ItemDatabase* itemDb);
    void          craftItem(Recipe* recipe, ItemDatabase& itemDb);
    bool          checkInventoryInput(float deltaTime,
                                      TouchData& touches, 
                                      ItemInstance** selectedItem, 
                                      bool& itemSelected, 
                                      Vector3D& itemPos,
                                      void** doubleClickCallbackData);

    bool          colidesWithRegion(GameMap& map, unsigned* regionIndex, unsigned* entryIndex);
    void          setPosition(Vector3D& position);
    void          resetPathIndex(){pathIndex = 0;}
    void          goToSleep();
    void          stopSleep(){sleeping = false;}

    int           isClothesEquiped();
    int           isWeaponEquiped();
    int           getAmmoInWeaponCount();
    int           getItemCountByTypeIndex(int index);

    unsigned      getItemCount(){return itemBag.getItemCount();}
    ItemInstance* getItem(unsigned index);

    Vector3D*     getPos(){return &pos;}
    int           getHealth(){return health;}
    int           getWarmth(){return warmth;}
    int           getSatiation(){return satiation;}
    int           getWakefullness(){return wakefullness;}

    bool          isSleeping(){return sleeping;}
    bool          isSleepAnimationDone(){return sleepAnimationDone;}

    virtual int   getType() override {return 0;}

    bool isColiding(Vector3D newPos, GameMap& map);

    void addItemToInventory(ItemInstance* item, int inventorySlotIndex);
    int  findFreedInventorySlot();
    bool isNoMorePlaceInBag(int freedSlotIndex);
    void addDoubleClickCallbackForItems(void (*func)(ItemInstance*, void**));
private:
    void wearClothes(float deltaTime, ItemDatabase& itemdb);
    void drainBatteries(float deltaTime, ItemDatabase& itemdb);
    
    //
    void doDarknessEffect(float deltaTime, float darkness);
    void doTemperatureDamage(float deltaTime, int temperature, ItemDatabase& itemdb);
    void doHungerDamage(float deltaTime);
    void walkingLogic(float deltaTime,
                      bool useKeys, 
                      unsigned char* Keys,
                      GameMap& map,
                      FindPath& path);

private:
    ItemContainer        itemBag;
    unsigned             pathIndex;
    bool                 playWalkAnimation;
    bool                 walkAnimationDone;
    bool                 sleeping;
    bool                 sleepAnimationDone;
    float                satiationProgress;
    float                freezingProgress;
    float                darknessProgress;
    float                timeAwake;

    ItemContainer        equipedItems;

    int                  satiation;
    float                wakefullness;
    float                warmth;
};


#endif //DUDE_H
