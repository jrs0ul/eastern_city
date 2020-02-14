#ifndef DUDE_H
#define DUDE_H

#include "Actor.h"
#include "../../Vectors.h"
#include "../../TextureLoader.h"
#include "../ActorContainer.h"
#include "../../FindPath.h"
#include "../map/GameMap.h"
#include "../FurnitureData.h"
#include "../CraftingRecipes.h"
#include "../ItemInstance.h"

class Dude : public Actor
{
    enum DudeImages
    {
        NAKED_IDLE = 22,
        NAKED_FLASHLIGHT = 23,
        NAKED_SLEEP = 24,
        COAT_IDLE = 10,
        COAT_FLASHLIGHT = 14,
        COAT_SLEEP = 17
    };


public:
    void          init(Vector3D& position, int ScreenWidth, int ScreenHeight);
    void          destroy();
    void          update(float deltaTime, 
                         unsigned char* Keys, 
                         GameMap& map,
                         Room* currentRoom,
                         ActorContainer& actors,
                         float darkness,
                         ItemDatabase& itemDb,
                         CraftingRecipes& recipes,
                         FurnitureDatabase& furnitureDb,
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
                                      bool& clickedOnInventory,
                                      Vector3D& itemPos,
                                      void** doubleClickCallbackData);

    bool          colidesWithRegion(GameMap& map, unsigned* regionIndex, unsigned* entryIndex);
    void          resetPathIndex(){pathIndex = 0;}
    void          activateMeleeAttack();
    void          goToSleep();
    void          stopSleep(){sleeping = false;}

    void          wearWeapon(float damage);
    void          setPosition(Vector3D& position);

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

    void melleeAttacks(ActorContainer& actors,
                       GameMap& map,
                       Room* currentRoom,
                       ItemDatabase& itemdb,
                       CraftingRecipes& recipes,
                       FurnitureDatabase& furnitureDb,
                       unsigned char* Keys,
                       float deltaTime
                       );

    void onWeaponEquip();
    void onWeaponUnequip();

private:
    ItemContainer        itemBag;
    unsigned             pathIndex;
    bool                 playWalkAnimation;
    bool                 walkAnimationDone;
    bool                 sleeping;
    bool                 sleepAnimationDone;
    bool                 doAttack;
    float                satiationProgress;
    float                freezingProgress;
    float                darknessProgress;
    float                timeAwake;

    ItemContainer        equipedItems;

    int                  satiation;
    float                wakefullness;
    float                warmth;

    int equipedWeaponInLastFrame;
};


#endif //DUDE_H
