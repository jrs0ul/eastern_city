#ifndef ITEM_CONTAINER_H
#define ITEM_CONTAINER_H

#include "ItemInstance.h"
#include "ItemDatabase.h"
#include "../TouchData.h"

class ItemContainer
{
public:
    void          init(unsigned sCount, unsigned newWidth);
    void          destroy();

    bool          addItem(ItemInstance& item, int slotIndex = -1);
    void          addItem(unsigned itemIndex); //for procedural generation;
    void          draw(PicsContainer& pics,
                       ItemDatabase& itemDB,
                       ItemInstance* selectedItem,
                       int scale,
                       bool showQuality);
    bool          checkInput(int scale,
                             float deltaTime,
                             TouchData& touches, 
                             ItemInstance** selectedItem, 
                             bool& itemSelected,
                             bool& clickedOnInventory,
                             Vector3D& itemPos,
                             void** callbackData);

    ItemInstance* getItem(unsigned index);
    int           hasItem(unsigned itemId);
    unsigned      getItemCount(){return items.count();}
    unsigned      getActualItemCount();
    unsigned      getSlotCount(){return slotCount;}
    bool          isActive(){return active;}

    void          setPosition(Vector3D position);
    void          setActive(bool act){active = act;}

    void          setDoubleClickCallback(void (*callback)(ItemInstance*, void**));
    
private:
    DArray<ItemInstance> items;
    unsigned slotCount;
    Vector3D pos;

    void (*doubleClickCallback)(ItemInstance*, void**);

    bool active;
    unsigned width;

    float doubleClickTimer;
    int selectedLocalItem;
    bool tappedTwoTimes;

    
};


#endif //ITEM_CONTAINER_H
