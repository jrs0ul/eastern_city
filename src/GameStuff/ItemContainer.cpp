#include "ItemContainer.h"
#include "../gui/Text.h"
#include <cmath>

void ItemContainer::init(unsigned sCount, unsigned newWidth)
{
    pos = Vector3D(0,0,0);
    slotCount = sCount;
    items.destroy();
    active = false;
    width = newWidth;
    selectedLocalItem = -1;
    tappedTwoTimes = false;
    doubleClickTimer = 0.f;
    doubleClickCallback = nullptr;
}

void ItemContainer::destroy()
{
    items.destroy();
}

bool ItemContainer::addItem(ItemInstance& item, int slotIndex)
{
    if (slotIndex == -1)
    {
        if (items.count() == slotCount)
        {
            return false;
        }

        ItemInstance itm = item;
        items.add(itm);
        return true;
    }

    if ((unsigned)slotIndex >= slotCount)
    {
        return false;
    }

    if ((unsigned)slotIndex < items.count())
    {
        if (!items[slotIndex].isRemoved())
        {
            return false;
        }

        items[slotIndex] = item;
    
    }
    else
    {
        for (unsigned i = items.count(); i < (unsigned)slotIndex; ++i)
        {
            ItemInstance itm;
            itm.setAsRemoved();
            items.add(itm);
        }

        items.add(item);
    }

    return true;
}

void ItemContainer::addItem(unsigned itemIndex)
{
    ItemInstance item;
    item.init(Vector3D(0,0,0), itemIndex);
    items.add(item);
}

void ItemContainer::draw(PicsContainer& pics, 
                         ItemDatabase& itemDB, 
                         ItemInstance* selectedItem,
                         int scale,
                         bool showQuality)
{

    unsigned col = 0;
    unsigned row = 0;

    for (unsigned i = 0; i < slotCount; ++i)
    {
        pics.draw(2, 
                  pos.x + (col * 34) * scale,
                  pos.y + (row * 34) * scale, 
                   0, 
                   false, 
                   0.25f * scale,
                   0.5 * scale);
        pics.draw(2, 
                  pos.x + (col * 34 + 16) * scale,
                  pos.y + (row * 34) * scale,
                  1, false, 0.25f * scale, 0.5 * scale);
        ++col;

        if (col >= width)
        {
            ++row;
            col = 0;
        }
    }


    col = 0;
    row = 0;

    char buf[50];

    for (unsigned i = 0; i < items.count(); ++i)
    {
        if (!items[i].isRemoved() && selectedItem != &items[i])
        {
            ItemData* data = itemDB.get(items[i].getIndex());
            pics.draw(4, pos.x + (col * 34) * scale,
                         pos.y + (row * 34) * scale, 
                         data->imageIndex,
                         false,
                         scale, scale);

            if (showQuality)
            {
                sprintf(buf, "%.1f", items[i].getQuality());
                WriteShadedText(pos.x + (col * 34) * scale,
                                pos.y + (row * 34 + 20) * scale, pics, 0, buf, 0.6f * scale, 0.6f * scale);
            }
        }

        ++col;

        if (col >= width)
        {
            ++row;
            col = 0;
        }
    }

}

bool ItemContainer::checkInput(int scale,
                               float deltaTime,
                               TouchData& touches,
                               ItemInstance** selectedItem, 
                               bool& itemSelected,
                               bool& clickedOnInventory,
                               Vector3D& itemPos,
                               void** callbackData)
{
    if (!active)
    {
        return false;
    }
    
    if (doubleClickTimer > 0.f)
    {
        doubleClickTimer -= deltaTime;
    }

    if (doubleClickTimer <= 0.f)
    {
        selectedLocalItem = -1;
    }

    const int height = ceil(slotCount / (width * 1.f));


    if (touches.down.count() && touches.down[0].x > pos.x && touches.down[0].x < pos.x + (34 * width) * scale
                    && touches.down[0].y > pos.y && touches.down[0].y < (height * 34) * scale + pos.y)
    {
        unsigned row = (touches.down[0].y - pos.y) / (34 * scale);
        unsigned col = (touches.down[0].x - pos.x) / (34 * scale);

        unsigned itemIndex = row * width + col;

        printf("col: %u row: %u itemIndex:%u\n", col, row, itemIndex);

        if (itemIndex < items.count() && !itemSelected)
        {
            if (!items[itemIndex].isRemoved())
            {
                *selectedItem = &items[itemIndex];
                itemSelected = true;
                itemPos = Vector3D(touches.down[0].x, touches.down[0].y, 0);

                tappedTwoTimes = (selectedLocalItem == (int)itemIndex);
                
                selectedLocalItem = (int)itemIndex;
                doubleClickTimer = 1.f;
                return true;
            }
        }


    }


    if (touches.up.count())
    {
        if (active 
                && (touches.up[0].x < pos.x || touches.up[0].x > pos.x + (34 * width) * scale
                    || touches.up[0].y < pos.y || touches.up[0].y > (height * 34) * scale + pos.y))
        {
            if (!itemSelected)
            {
                active = false;
            }

            return !itemSelected;
        }
        else if (touches.up[0].x > pos.x && touches.up[0].x < pos.x + (34 * width) * scale
                    && touches.up[0].y > pos.y && touches.up[0].y < (height * 34) * scale + pos.y)
        {
            clickedOnInventory = true;

            if (itemSelected && active)
            {

                unsigned row = (touches.up[0].y - pos.y) / (34 * scale);
                unsigned col = (touches.up[0].x - pos.x) / (34 * scale);

                unsigned itemIndex = row * width + col;

                if (addItem(**selectedItem, itemIndex))
                {
                    (**selectedItem).setAsRemoved();
                    printf("REMOVED\n");
                }

                itemSelected = false;
                *selectedItem = nullptr;

                if (tappedTwoTimes)
                {
                    if (doubleClickCallback)
                    {
                        (*doubleClickCallback)(&items[itemIndex], callbackData);
                    }
                    selectedLocalItem = -1;
                    tappedTwoTimes = false;
                }

                return true;
            }

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

int ItemContainer::hasItem(unsigned itemId)
{
    for (unsigned j = 0; j < items.count(); ++j)
    {
        if (items[j].getIndex() == (int)itemId && !items[j].isRemoved())
        {
            return j;
        }
    }

    return -1;

}

unsigned ItemContainer::getActualItemCount()
{
    unsigned count = 0;

    for (unsigned i = 0; i < items.count(); ++i)
    {
        if (!items[i].isRemoved())
        {
            ++count;
        }
    }

    return count;
}

void ItemContainer::setPosition(Vector3D position)
{
    pos = position;
}

void ItemContainer::setDoubleClickCallback(void (*callback)(ItemInstance*, void**))
{
    doubleClickCallback = callback;
}

