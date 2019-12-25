#include "Dude.h"
#include "../TextureLoader.h"
#include "../Usefull.h"
#include <cmath>

void Dude::init(Vector3D& position)
{
    pathIndex = 0;
    animationFrame = 0;
    animationSubset = 0;
    animationProgress = 0.f;
    isFlipedX = false;
    pos = position;
    playWalkAnimation = false;
    walkAnimationDone = true;
    maxItems = 10;
    satiationProgress = 0.f;
    freezingProgress = 0.f;
    satiation = 100;
    health = 100.f;
    equipedBodyItem = nullptr;
    collisionBodyOffset = Vector3D(0.f, 39.f, 0.f);
    collisionBodyRadius = 16.f;
    strcpy(spriteName, "pics/dude.tga");

    FrameSet up;
    up.frames.add(0);
    up.frames.add(1);
    up.frames.add(2);
    up.frames.add(1);
    FrameSet down;
    down.frames.add(3);
    down.frames.add(4);
    down.frames.add(5);
    down.frames.add(4);
    FrameSet side;
    side.frames.add(6);
    side.frames.add(7);
    side.frames.add(8);
    side.frames.add(7);

    animations.add(up);
    animations.add(down);
    animations.add(side);
}

void Dude::destroy()
{
    items.destroy();
    Actor::destroy();
}

void Dude::update(float deltaTime, 
                  unsigned char* Keys,
                  GameMap& map,
                  ItemDatabase& itemdb,
                  Path& path)
{

    ItemData* clothingInfo = nullptr;

    if (equipedBodyItem)
    {
        clothingInfo = itemdb.get(equipedBodyItem->getIndex());
        equipedBodyItem->setQuality(equipedBodyItem->getQuality() - deltaTime * clothingInfo->spoilageSpeed);


        //printf("clothing quality: %f\n", equipedBodyItem->getQuality());

        if (equipedBodyItem->getQuality() <= 0.f)
        {
            equipedBodyItem->setAsRemoved();
            equipedBodyItem = nullptr;
        }
    }

    satiationProgress += deltaTime;

    freezingProgress += deltaTime * 1.2f;

    if (satiationProgress >= 1.f)
    {
        if (satiation > 0)
        {
            --satiation;
        }
        else
        {
            if (health > 0.f)
            {
                health -= 1.f;
            }
        }

        satiationProgress = 0.f;
    }

    if (freezingProgress >= 1.f)
    {

        int clothingTemperature = 0;

        if (equipedBodyItem)
        {
            clothingTemperature = itemdb.get(equipedBodyItem->getIndex())->coldDecrease;
        }

        float temperatureDamage = (map.getTemperature() + clothingTemperature) / 20.f;

        if (temperatureDamage < 0.f)
        {
            //printf("Temperature damage %f\n", temperatureDamage);
            health += temperatureDamage;
        }

        freezingProgress = 0.f;
    }
    
    //-----

    float dudeSpeed = 1.8f;
    const bool useKeys = Keys[0] + Keys[1] + Keys[2] + Keys[3];

    if (useKeys)
    {
        path.destroy();
    }

    const bool followPath = pathIndex < (int)path.parent.count();

    if ((useKeys || followPath) && walkAnimationDone)
    {
        playWalkAnimation = true;
        walkAnimationDone = false;
    }
    else if (walkAnimationDone)
    {
        animationFrame = 1;
    }


    Vector3D shift(0, 0, 0);

    if (!useKeys)
    {
        if (followPath)
        {
            Vector3D destination = Vector3D(path.parent[pathIndex].x * 32 + 16,
                                            path.parent[pathIndex].y * 32 + 16,
                                            0);
            Vector3D shiftedPos = pos + Vector3D(0, 39, 0);
            Vector3D direction = destination - shiftedPos;
            direction.normalize();
            shift = Vector3D(direction.x * dudeSpeed, direction.y * dudeSpeed, 0);

            Vector3D tmp = shiftedPos + shift;

            if (CollisionCircleCircle(tmp.x, tmp.y, 2, destination.x, destination.y, 1))
            {
                ++pathIndex;
            }
        }
    }
    else
    {

        if (Keys[0])
        {
            shift.y = -dudeSpeed;
        }
        else if (Keys[1])
        {
            shift.y = dudeSpeed;
        }

        if (Keys[2])
        {
            shift.x = -dudeSpeed;
        }
        else if (Keys[3])
        {
            shift.x = dudeSpeed;
        }
    }

    if (fabsf(shift.y) > fabsf(shift.x))
    {
        if (shift.y < 0.f)
        {
            animationSubset = 1;
        }
        else if (shift.y > 0.f)
        {
            animationSubset = 0;
        }

        if (Actor::isColiding(pos + shift, map))
        {
            shift.y = 0.f;
        }
    }

    if (fabsf(shift.y) < fabsf(shift.x))
    {
        if (shift.x < 0.f)
        {
            animationSubset = 2;
            isFlipedX = false;
        }
        else if (shift.x > 0.f)
        {
            animationSubset = 2;
            isFlipedX = true;
        }

        if (Actor::isColiding(pos + shift, map))
        {
            shift.x = 0.f;
        }
    }


    Vector3D newPos = pos + shift;

    if (!Actor::isColiding(newPos, map))
    {
        pos = newPos;
    }

    //---

    if (playWalkAnimation && walkAnimationDone == false)
    {
        animationProgress += deltaTime * 5.f;

        if (animationProgress >= 1.f)
        {
            ++animationFrame;
            animationProgress = 0.f;
            playWalkAnimation = false;
            walkAnimationDone = true;

            if (animationFrame > 3)
            {
                animationFrame = 0;
            }
        }
    }

    //picking up items ----
    Vector3D shiftedPos = pos + Vector3D(0, 39, 0);

    int replacableItemIndex = findFreedInventorySlot();

    for (int i = 0; i < map.getItemCount(); ++i)
    {
        ItemInstance* itm = map.getItem(i);

        bool noMorePlaceInBag = isNoMorePlaceInBag(replacableItemIndex);
        
        if (itm->isRemoved() || noMorePlaceInBag)
        {
            continue;
        }

        if (CollisionCircleCircle(shiftedPos.x, shiftedPos.y, 16, itm->getPosition()->x, itm->getPosition()->y, 8))
        {
            addItemToInventory(itm, replacableItemIndex);
            itm->setAsRemoved();
        }
    }

}

void Dude::drawInventory(PicsContainer& pics, ItemDatabase& itemDb)
{
    for (int i = 0; i < maxItems; ++i)
    {
        pics.draw(2, 100 + i * 34, 445, 0, false, 0.25f, 0.5);
        pics.draw(2, 100 + i * 34 + 16, 445, 1, false, 0.25f, 0.5);

        if (i < (int)items.count())
        {
            if (items[i].isRemoved())
            {
                continue;
            }

            ItemData* data = itemDb.get(items[i].getIndex());
            pics.draw(4, 100 + i * 34, 445, data->imageIndex);
        }
    }
}

void Dude::useItem(unsigned index, ItemDatabase& itemDb)
{
    if (index < items.count() && !items[index].isRemoved())
    {
        ItemData* data = itemDb.get(items[index].getIndex());

        if (!data->isWearable)
        {
            if (!data->isConsumable)
            {
                return;
            }

            printf("satiationUp:%d hpUp:%d\n", data->hungerDecrease, data->hpUp);

            satiation += data->hungerDecrease;

            if (satiation > 100)
            {
                satiation = 100; 
            }
            
            health += data->hpUp;

            if (health > 100)
            {
                health = 100;
            }
            

            items[index].setAsRemoved();
        }
        else
        {
            equipedBodyItem = &items[index];
        }
    }
}

void Dude::craftItem(Recipe* recipe, ItemDatabase& itemDb)
{
    if (!recipe)
    {
        return;
    }

    int ingredientCount = recipe->ingredients.count();
    DArray<int> itemsToRemove;

    for (unsigned i = 0; i < recipe->ingredients.count(); ++i)
    {
        Ingredient* ingredient = &recipe->ingredients[i];

        int itemCountToFind = ingredient->count;

        for (unsigned j = 0; j < items.count(); ++j)
        {
            if (items[j].getIndex() == ingredient->itemIndex && !items[j].isRemoved())
            {
                itemsToRemove.add(j);
                --itemCountToFind;
                if (!itemCountToFind)
                {
                    --ingredientCount;
                    break;
                }
            }
        }

        if (itemCountToFind)
        {
            printf("You don't have required items\n");
            return;
        }

    }

    if (ingredientCount)
    {
        printf("You don't have required items\n");
        itemsToRemove.destroy();
        return;

    }

    for (unsigned i = 0; i < itemsToRemove.count(); ++i)
    {
        items[itemsToRemove[i]].setAsRemoved();
    }

    ItemInstance craftedItem;
    craftedItem.init(Vector3D(0, 0, 0), recipe->indexOfItemToMake);

    int freeSlot = findFreedInventorySlot();
    if (!isNoMorePlaceInBag(freeSlot))
    {
        addItemToInventory(&craftedItem, freeSlot);
    }

    itemsToRemove.destroy();
}

bool Dude::colidesWithRegion(GameMap& map, unsigned* regionIndex, unsigned* entryIndex)
{
    Vector3D newPos = pos + Vector3D(0, 39, 0);

    for (int i = 0; i < map.getRegionCount(); ++i)
    {
        Region* reg = map.getRegion(i);

        
        if (CollisionCircleRectangle(newPos.x, newPos.y, 15.f, 
                                     reg->pos.x, reg->pos.y, reg->size.x, reg->size.y))
        {
            (*regionIndex) = i;
            (*entryIndex) = reg->entryIndex;
            //printf("colided [%s %u]\n", reg->name, reg->entryIndex);
            return true;
        }
    }

    return false;
}


void Dude::setPosition(Vector3D& position)
{
    pos = position;
}

ItemInstance* Dude::getItem(unsigned index)
{
    if (index < items.count())
    {
        return &items[index];
    }

    return nullptr;
}

void Dude::addItemToInventory(ItemInstance* item, int inventorySlotIndex)
{
    if (inventorySlotIndex != -1)
    {
        items[inventorySlotIndex] = *item;
    }
    else if ((int)items.count() < maxItems)
    {
        ItemInstance itemToAdd = *item;
        items.add(itemToAdd);
    }

}


int Dude::findFreedInventorySlot()
{
    int replacableItemIndex = -1;

    for (unsigned i = 0; i < items.count(); ++i)
    {
        if (items[i].isRemoved())
        {
            replacableItemIndex = i;
            break;
        }
    }

    return replacableItemIndex;

}

bool Dude::isNoMorePlaceInBag(int freedSlotIndex)
{
    return ((int)items.count() >= maxItems) ? (freedSlotIndex == -1 ?  true : false) : false;
}
