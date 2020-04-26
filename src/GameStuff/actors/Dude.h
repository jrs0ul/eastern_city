#ifndef DUDE_H
#define DUDE_H

#include "Actor.h"

class  FindPath;
class  CraftingRecipes;
struct Recipe;
class  FurnitureDatabase;
class  ItemDatabase;
class  ItemInstance;
struct TouchData;
class  ItemContainer;
class  Furniture;

class Dude : public Actor
{
    enum DudeImages
    {
        NAKED_IDLE = 22,
        NAKED_FLASHLIGHT = 23,
        NAKED_SLEEP = 24,
        NAKED_AXE = 26,
        COAT_IDLE = 10,
        COAT_FLASHLIGHT = 14,
        COAT_SLEEP = 17,
        COAT_AXE = 25,
        COAT_SLINGSHOT = 28,
        NAKED_SLINGSHOT = 30
    };


public:

    Dude() : itemBag(nullptr), equipedItems(nullptr){}

    void          draw(float offsetX, float offsetY,
                         int scale,
                         PicsContainer& pics, 
                         bool debugInfo = false) override;


    void          init(Vector3D& position,
                       Room* currentRoom,
                       GameMap* currentMap,
                       int ScreenWidth, 
                       int ScreenHeight, 
                       int scale);
    void          destroy();
    void          update(float deltaTime, 
                         unsigned char* Keys, 
                         GameMap& map,
                         Room* currentRoom,
                         float darkness,
                         ItemDatabase& itemDb,
                         CraftingRecipes& recipes,
                         FurnitureDatabase& furnitureDb,
                         FindPath& path);

    void          drawInventory(PicsContainer& pics,
                                ItemDatabase& itemDb,
                                ItemInstance* selectedItem,
                                int scale);
    int           hasItem(unsigned itemId);
    void          removeItem(unsigned index);
    void          useItem(ItemInstance*, ItemDatabase* itemDb);
    void          craftItem(Recipe* recipe, ItemDatabase& itemDb);
    bool          checkInventoryInput(int scale,
                                      float deltaTime,
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

    unsigned      getItemCount();
    ItemInstance* getItem(unsigned index);

    int           getWarmth(){return warmth;}
    int           getSatiation(){return satiation;}
    int           getWakefullness(){return wakefullness;}
    Vector3D*     getDirection(){return &direction;}
    SPolygon*     getCurrentHelperPolygon();

    bool          isSleeping(){return sleeping;}
    bool          isSleepAnimationDone(){return sleepAnimationDone;}

    virtual int   getType() override {return 0;}

    void          addItemToInventory(ItemInstance* item, int inventorySlotIndex);
    int           findFreedInventorySlot();
    bool          isNoMorePlaceInBag(int freedSlotIndex);
    void          addDoubleClickCallbackForItems(void (*func)(ItemInstance*, void**));
    void          setAnimationSubsetByDirectionVector(Vector3D direction, bool useAxeAnims = true);
private:
    void          wearClothes(float deltaTime, ItemDatabase& itemdb);
    void          drainBatteries(float deltaTime, ItemDatabase& itemdb);
    
    //
    void doDarknessEffect(float deltaTime, float darkness);
    void doTemperatureDamage(float deltaTime, int temperature, ItemDatabase& itemdb);
    void doHungerDamage(float deltaTime);
    void walkingLogic(float deltaTime,
                      bool useKeys, 
                      unsigned char* Keys,
                      GameMap& map,
                      FindPath& path);

    void melleeAttacks(GameMap& map,
                       Room* currentRoom,
                       ItemDatabase& itemdb,
                       CraftingRecipes& recipes,
                       FurnitureDatabase& furnitureDb,
                       unsigned char* Keys,
                       float deltaTime
                       );

    void destroyFurniture(Furniture* fur,
                          Room* currentRoom,
                          GameMap& map,
                          CraftingRecipes& recipes);

    void onWeaponEquip();
    void onWeaponUnequip();

private:
    ItemContainer*       itemBag;
    Vector3D             attackBoxPos;
    Vector3D             attackBoxSize;
    Vector3D             direction;
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

    ItemContainer*       equipedItems;

    int                  satiation;
    float                wakefullness;
    float                warmth;

    int equipedWeaponInLastFrame;
};


#endif //DUDE_H
