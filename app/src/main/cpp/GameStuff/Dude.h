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
    void          update(float deltaTime, unsigned char* Keys, GameMap& map, ItemDatabase& itemDb, Path& path);

    void          drawInventory(PicsContainer& pics, ItemDatabase& itemDb, ItemInstance* selectedItem);
    int           hasItem(unsigned itemId);
    void          removeItem(unsigned index);
    void          useItem(unsigned index, ItemDatabase& itemDb);
    void          craftItem(Recipe* recipe, ItemDatabase& itemDb);
    bool          checkInventoryInput(TouchData& touches, 
                                      ItemInstance** selectedItem, 
                                      bool& itemSelected, 
                                      Vector3D& itemPos);

    bool          colidesWithRegion(GameMap& map, unsigned* regionIndex, unsigned* entryIndex);
    void          setPosition(Vector3D& position);
    void          setHealth(float newHealth){health = newHealth;}
    void          resetPathIndex(){pathIndex = 0;}

    int           isWeaponEquiped(){return (equipedWeapon.isRemoved()) ? -1 : equipedWeapon.getIndex();}
    int           hasWeaponAmmo(){return (equipedWeapon.isRemoved()) ? 0 : equipedWeapon.getAmmoLoaded();}

    unsigned      getItemCount(){return itemBag.getItemCount();}
    ItemInstance* getItem(unsigned index);

    Vector3D*     getPos(){return &pos;}
    int           getHealth(){return health;}
    int           getSatiation(){return satiation;}

    virtual int   getType() override {return 0;}
private:

    bool isColiding(Vector3D newPos, GameMap& map);

    void addItemToInventory(ItemInstance* item, int inventorySlotIndex);
    int  findFreedInventorySlot();
    bool isNoMorePlaceInBag(int freedSlotIndex);
    void wearClothes(float deltaTime, ItemDatabase& itemdb);
    void drainBatteries(float deltaTime, ItemDatabase& itemdb);
    
    //
    void doTemperatureDamage(float deltaTime, int temperature, ItemDatabase& itemdb);
    void doHungerDamage(float deltaTime);

private:
    ItemContainer        itemBag;
    int                  pathIndex;
    bool                 playWalkAnimation;
    bool                 walkAnimationDone;
    float                satiationProgress;
    float                freezingProgress;

    ItemInstance         equipedClothes;
    ItemInstance         equipedWeapon;

    int                  satiation;
};


#endif //DUDE_H
