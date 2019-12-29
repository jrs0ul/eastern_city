#include "ItemContainer.h"

void ItemContainer::init(unsigned sCount)
{
    pos = Vector3D(0,0,0);
    slotCount = sCount;
    items.destroy();
    active = false;
}

void ItemContainer::destroy()
{
    items.destroy();
}

void ItemContainer::addItem(ItemInstance& item)
{
    items.add(item);
}

void ItemContainer::draw(PicsContainer& pics, ItemDatabase& itemDB, ItemInstance* selectedItem)
{
     for (unsigned i = 0; i < slotCount; ++i)
    {
        pics.draw(2, pos.x, pos.y + i * 34, 0, false, 0.25f, 0.5);
        pics.draw(2, pos.x + 16, pos.y + i * 34, 1, false, 0.25f, 0.5);
    }

    for (unsigned i = 0; i < items.count(); ++i)
    {
        if (!items[i].isRemoved() && selectedItem != &items[i])
        {
            ItemData* data = itemDB.get(items[i].getIndex());
            pics.draw(4, pos.x, pos.y + i * 34, data->imageIndex);
        }
    }

}

bool ItemContainer::checkInput(TouchData& touches, ItemInstance** selectedItem, bool& itemSelected, Vector3D& itemPos)
{
    if (!active)
    {
        return false;
    }

    if (touches.down.count())
    {
        for (unsigned i = 0; i < slotCount; ++i)
        {
            if (touches.down[0].x > pos.x && touches.down[0].x < pos.x + 32 
                && touches.down[0].y > pos.y + i * 34 && touches.down[0].y < pos.y + i * 34 + 32 
                && !itemSelected
               )
            {
                if (i < items.count() && !items[i].isRemoved())
                {
                    *selectedItem = &items[i];
                    itemSelected = true;
                    itemPos = Vector3D(touches.down[0].x, touches.down[0].y, 0);
                    return true;
                }
            }

        }

    }


    if (touches.up.count())
    {

        if (active 
                && (touches.up[0].x < pos.x || touches.up[0].x > pos.x + 32
                    || touches.up[0].y < pos.y || touches.up[0].y > slotCount * 34 + pos.y))
        {
            active = false;
            return !itemSelected;
        }


    }

    return false;
}

ItemInstance* ItemContainer::getItem(unsigned index)
{
    if (index < items.count())
    {
        return &items[index];
    }

    return nullptr;
}

void ItemContainer::setPosition(Vector3D position)
{
    pos = position;
}

