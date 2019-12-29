#ifndef ITEM_CONTAINER_H
#define ITEM_CONTAINER_H

#include "ItemInstance.h"
#include "ItemDatabase.h"
#include "../TouchData.h"

class ItemContainer
{
public:
    void          init(unsigned sCount);
    void          destroy();

    void          addItem(ItemInstance& item);
    void          draw(PicsContainer& pics, ItemDatabase& itemDB, ItemInstance* selectedItem);
    bool          checkInput(TouchData& touches, ItemInstance** selectedItem, bool& itemSelected, Vector3D& itemPos);

    ItemInstance* getItem(unsigned index);
    unsigned      getItemCount(){return items.count();}
    unsigned      getSlotCount(){return slotCount;}
    bool          isActive(){return active;}

    void          setPosition(Vector3D position);
    void          setActive(bool act){active = act;}
    
private:
    DArray<ItemInstance> items;
    unsigned slotCount;
    Vector3D pos;
    bool active;
};


#endif //ITEM_CONTAINER_H
