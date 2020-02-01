#include "Dude.h"
#include "Statistics.h"
#include "../TextureLoader.h"
#include "../Useful.h"
#include "../gui/Text.h"
#include <cmath>

void Dude::init(Vector3D& position)
{
    pathIndex = 0;
    animationFrame = 0;
    animationSubset = 0;
    animationProgress = 0.f;
    darknessProgress = 0.f;
    damageProgress = 0.f;
    isDamaged = false;
    isFlipedX = false;
    pos = position;
    playWalkAnimation = false;
    walkAnimationDone = true;
    sleeping = false;
    sleepAnimationDone = true;
    itemBag.init(10, 10);
    itemBag.setPosition(Vector3D(100, 445, 0));
    equipedItems.init(2, 2);
    equipedItems.setPosition(Vector3D(746, 445,0));
    satiationProgress = 0.f;
    freezingProgress = 0.f;
    satiation = 100;
    wakefullness = 100;
    warmth = 100.f;
    health = 100.f;
    timeAwake = 0.f;
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

    FrameSet sleep;
    sleep.offsetX = -17;
    sleep.frames.add(0);
    sleep.frames.add(1);
    sleep.frames.add(2);
    sleep.frames.add(3);

    animations.add(up);
    animations.add(down);
    animations.add(side);
    animations.add(sleep);
}

void Dude::destroy()
{
    itemBag.destroy();
    equipedItems.destroy();
    Actor::destroy();
}

void Dude::update(float deltaTime, 
                  unsigned char* Keys,
                  GameMap& map,
                  float darkness,
                  ItemDatabase& itemdb,
                  FindPath& path)
{

    if (isDamaged)
    {
        damageProgress -= deltaTime;
        
        if (damageProgress <= 0.f)
        {
            isDamaged = false;
        }
    }

    //Y U DO THIS???
    if (isWeaponEquiped() == 12 && !sleeping)
    {
        strcpy(spriteName, "pics/dude_flashlight.tga");
    }
    else if (sleeping)
    {
        strcpy(spriteName,"pics/dude_sleep.tga");
    }
    else
    {
        strcpy(spriteName, "pics/dude.tga");
    }
    //-----------

    wearClothes(deltaTime, itemdb);
    drainBatteries(deltaTime, itemdb);
    doDarknessEffect(deltaTime, darkness);
    doHungerDamage(deltaTime);
    doTemperatureDamage(deltaTime, map.getTemperature(), itemdb);

    const bool useKeys = Keys[0] + Keys[1] + Keys[2] + Keys[3];

    if (useKeys)
    {
        path.destroy();

        if (sleeping && sleepAnimationDone)
        {
            sleeping = false;
        }

    }

    if (!sleeping)
    {
        walkingLogic(deltaTime, useKeys, Keys, map, path);
    }

    
    if (sleeping && sleepAnimationDone == false)
    {
        animationProgress += deltaTime * 5.f;

        if (animationProgress >= 1.f)
        {
            ++animationFrame;
            animationProgress = 0.f;

            if (animationFrame > 3)
            {
                animationFrame = 3;
                sleepAnimationDone = true;
            }
        }
    }

    //picking up items ----
    Vector3D shiftedPos = pos + Vector3D(0, 39, 0);


    for (int i = 0; i < map.getItemCount(); ++i)
    {
        ItemInstance* itm = map.getItem(i);

        int replacableItemIndex = findFreedInventorySlot();
        bool noMorePlaceInBag = isNoMorePlaceInBag(replacableItemIndex);

        if (noMorePlaceInBag)
        {
            return;
        }
        
        if (itm->isRemoved())
        {
            continue;
        }

        if (CollisionCircleCircle(shiftedPos.x, shiftedPos.y, 16, itm->getPosition()->x, itm->getPosition()->y, 8))
        {
            printf("adding to slot %d\n", replacableItemIndex);
            addItemToInventory(itm, replacableItemIndex);
            Statistics::getInstance()->increaseItemAddition();
            itm->setAsRemoved();
        }
    }

}

void Dude::drawInventory(PicsContainer& pics, ItemDatabase& itemDb, ItemInstance* selectedItem)
{
    itemBag.draw(pics, itemDb, selectedItem);
    equipedItems.draw(pics, itemDb, selectedItem);

    
    char buf[100];

    if (isClothesEquiped() != -1)
    {
        sprintf(buf, "%.1f", equipedItems.getItem(0)->getQuality());
        WriteShadedText(746, 445 + 20, pics, 0, buf, 0.6f, 0.6f);
    }

    if (isWeaponEquiped() != -1)
    {
        sprintf(buf, "%.1f", equipedItems.getItem(1)->getQuality());
        WriteShadedText(746 + 34, 445 + 20, pics, 0, buf, 0.6f, 0.6f);
    }
}


int Dude::hasItem(unsigned itemId)
{
    return itemBag.hasItem(itemId);
}

void Dude::removeItem(unsigned index)
{
    ItemInstance* itm = itemBag.getItem(index);

    if (itm)
    {
        itm->setAsRemoved();
    }
}

void Dude::useItem(ItemInstance* item, ItemDatabase* itemDb)
{
    if (!item || item->isRemoved())
    {
        printf("shit item\n");
        return;
    }


    ItemData* data = itemDb->get(item->getIndex());

    if (data->isConsumable)
    {
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

        if (data->clothingQualityIncrease > 0.f)
        {
            if (isClothesEquiped() != -1)
            {
                float quality = equipedItems.getItem(0)->getQuality();
                quality += data->clothingQualityIncrease;

                if (quality > 100.f)
                {
                    quality = 100.f;
                }

                equipedItems.getItem(0)->setQuality(quality);
            }
            else
            {
                return;
            }
        }

        item->setAsRemoved();
        Statistics::getInstance()->increaseItemUsage();
    }
    else if (data->isWearable)
    {
        if (equipedItems.addItem(*item, 0))
        {
            item->setAsRemoved();
            Statistics::getInstance()->increaseEquipedTimes();
        }
    }
    else if (data->isWeapon)
    {
        printf("Using weapon\n");
        if (isWeaponEquiped() != -1)
        {
            printf("has item in weapon slot\n");

            int freedSlot = findFreedInventorySlot();
            if (isNoMorePlaceInBag(freedSlot))
            {
                return;
            }

            addItemToInventory(equipedItems.getItem(1), freedSlot);

            equipedItems.getItem(1)->setAsRemoved();
        }

        if (equipedItems.addItem(*item, 1))
        {
            item->setAsRemoved();
            Statistics::getInstance()->increaseEquipedTimes();
        }
    }
    else if (data->imageIndex == 13) // batteries
    {
        if (isWeaponEquiped() != -1 && isWeaponEquiped() == 12)
        {
            equipedItems.getItem(1)->setAmmoLoaded(1);
            equipedItems.getItem(1)->setQuality(100.f);
            item->setAsRemoved();
            Statistics::getInstance()->increaseItemUsage();
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

        for (unsigned j = 0; j < itemBag.getItemCount(); ++j)
        {
            ItemInstance* itm = itemBag.getItem(j);

            if (itm && itm->getIndex() == ingredient->itemIndex && !itm->isRemoved())
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
        itemBag.getItem(itemsToRemove[i])->setAsRemoved();
    }

    ItemInstance craftedItem;
    craftedItem.init(Vector3D(0, 0, 0), recipe->indexOfItemToMake);

    int freeSlot = findFreedInventorySlot();
    if (!isNoMorePlaceInBag(freeSlot))
    {
        addItemToInventory(&craftedItem, freeSlot);
    }

    itemsToRemove.destroy();
    Statistics::getInstance()->increaseCraftedItems();
}

bool Dude::checkInventoryInput(float deltaTime,
                               TouchData& touches, 
                               ItemInstance** selectedItem, 
                               bool& itemSelected, 
                               Vector3D& itemPos,
                               void** doubleClickCallbackData)
{
    itemBag.setActive(true);
    unsigned  itemsBefore = itemBag.getActualItemCount();
    bool res = itemBag.checkInput(deltaTime, touches, selectedItem, itemSelected, itemPos, doubleClickCallbackData);
    unsigned itemsAfter = itemBag.getActualItemCount();
    
    if (itemsBefore < itemsAfter)
    {
        Statistics::getInstance()->increaseItemAddition();
    }

    equipedItems.setActive(true);
    itemsBefore = equipedItems.getActualItemCount();
    res = equipedItems.checkInput(deltaTime, touches, selectedItem, itemSelected, itemPos, nullptr);
    itemsAfter = equipedItems.getActualItemCount();
    
    if (itemsBefore < itemsAfter)
    {
        Statistics::getInstance()->increaseEquipedTimes();
    }


    //printf("%d\n", res);
    return res;
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

void Dude::goToSleep()
{
    sleeping = true; 
    sleepAnimationDone = false;
    animationSubset = 3; 
    animationFrame = 0;
    timeAwake = 0.f;
}

void Dude::setPosition(Vector3D& position)
{
    pos = position;
}

int Dude::isClothesEquiped()
{
    ItemInstance* clothes = equipedItems.getItem(0);

    if (!clothes)
    {
        return -1;
    }

    return (clothes->isRemoved()) ? -1 : clothes->getIndex();
}


int Dude::isWeaponEquiped()
{
    ItemInstance* weapon = equipedItems.getItem(1);
    if (!weapon)
    {
        return -1;
    }

    return (weapon->isRemoved()) ? -1 : weapon->getIndex();
}

int  Dude::getAmmoInWeaponCount()
{
    ItemInstance* weapon = equipedItems.getItem(1);

    if (!weapon)
    {
        return 0;
    }

    return (weapon->isRemoved()) ? 0 : weapon->getAmmoLoaded();
}


int Dude::getItemCountByTypeIndex(int index)
{
    int count = 0;

    for (unsigned j = 0; j < itemBag.getItemCount(); ++j)
    {
        ItemInstance* itm = itemBag.getItem(j);

        if (itm && itm->getIndex() == index && !itm->isRemoved())
        {
            ++count;
        }
    }

    return count;
}

ItemInstance* Dude::getItem(unsigned index)
{
    return itemBag.getItem(index);
}

void Dude::addItemToInventory(ItemInstance* item, int inventorySlotIndex)
{
    itemBag.addItem(*item, inventorySlotIndex);
}


int Dude::findFreedInventorySlot()
{
    int replacableItemIndex = -1;

    for (unsigned i = 0; i < itemBag.getItemCount(); ++i)
    {
        ItemInstance* itm = itemBag.getItem(i);

        if (itm->isRemoved())
        {
            replacableItemIndex = i;
            break;
        }
    }

    return replacableItemIndex;

}

bool Dude::isNoMorePlaceInBag(int freedSlotIndex)
{
    return (itemBag.getItemCount() >= itemBag.getSlotCount()) ? (freedSlotIndex == -1 ?  true : false) : false;
}

void Dude::addDoubleClickCallbackForItems(void (*func)(ItemInstance*, void**))
{
    itemBag.setDoubleClickCallback(func);
}

void Dude::wearClothes(float deltaTime, ItemDatabase& itemdb)
{
    ItemData* clothingInfo = nullptr;

    if (isClothesEquiped() != -1)
    {
        ItemInstance* clothes = equipedItems.getItem(0);
        clothingInfo = itemdb.get(clothes->getIndex());
        clothes->setQuality(clothes->getQuality() - deltaTime * clothingInfo->spoilageSpeed);


        //printf("clothing quality: %f\n", equipedBodyItem->getQuality());

        if (clothes->getQuality() <= 0.f)
        {
            clothes->setAsRemoved();
        }
    }
}

void Dude::drainBatteries(float deltaTime, ItemDatabase& itemdb)
{
    if (isWeaponEquiped() != -1)
    {
        ItemInstance* weapon = equipedItems.getItem(1);
        int itemID = weapon->getIndex();

        if (itemID != 12)
        {
            return; //only works with flashlight
        }

        ItemData* equipedItemInfo = itemdb.get(itemID);

        if (equipedItemInfo)
        {
            ItemData* ammoInfo = itemdb.get(equipedItemInfo->ammoItemIndex);

            if (weapon->getAmmoLoaded())
            {
                weapon->setQuality(weapon->getQuality() - deltaTime * ammoInfo->spoilageSpeed);

                if (weapon->getQuality() <= 0.f)
                {
                    weapon->setQuality(0.f);
                    weapon->setAmmoLoaded(0);
                }
            }
        }

    }
}

void Dude::doDarknessEffect(float deltaTime, float darkness)
{

    if (sleeping)
    {
        wakefullness += deltaTime * 1.3f;

        if (wakefullness > 100)
        {
            wakefullness = 100;
        }
           
        return;
    }


    timeAwake += deltaTime;
    
    if (timeAwake > 40)
    {
        darknessProgress += deltaTime;

        if (darknessProgress >= 1.f)
        {
            int hitCount = (int)darknessProgress;

            for (int i = 0; i < hitCount; ++i)
            {

                wakefullness -= darkness * 2;

                if (wakefullness <= 0)
                {
                    wakefullness = 0;

                    if (health > 0.f)
                    {
                        health -= 1.f;
                        isDamaged = true;
                    }
                }

                darknessProgress = 0.f;
            }
        }
    }

}

void Dude::doTemperatureDamage(float deltaTime, int temperature, ItemDatabase& itemdb)
{
    freezingProgress += deltaTime * 1.2f;

    if (freezingProgress >= 1.f)
    {
        int hitCount = (int)freezingProgress;

        int clothingTemperature = 0;

        if (isClothesEquiped() != -1)
        {
            clothingTemperature = itemdb.get(equipedItems.getItem(0)->getIndex())->coldDecrease;
        }

        for (int i = 0; i < hitCount; ++i)
        {
            float temperatureDamage = ((temperature + clothingTemperature) - 15) / 5.f;

            warmth += temperatureDamage;

            if (warmth > 100.f)
            {
                warmth = 100.f;
            }

            if (warmth <= 0.f)
            {
                warmth = 0.f;
                health -= 1;

                if (health < 0)
                {
                    health = 0;
                }

                isDamaged = true;
                damageProgress = 0.1f;
            }

            freezingProgress = 0.f;
        }
    }

}

void Dude::doHungerDamage(float deltaTime)
{
    satiationProgress += deltaTime;

    if (satiationProgress >= 1.f)
    {
        int hitCount = (int)satiationProgress;

        for (int i = 0; i < hitCount; ++i)
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
                    isDamaged = true;
                    damageProgress = 0.1f;
                }
            }
        }

        satiationProgress = 0.f;
    }
}

void Dude::walkingLogic(float deltaTime, bool useKeys, unsigned char* Keys, GameMap& map, FindPath& path)
{
    float dudeSpeed = 1.8f;

    const bool followPath = pathIndex < path.getPathLength();

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
            Vector3D* destination = path.getPathStep(pathIndex);

            if (destination)
            {
                Vector3D shiftedPos = pos + Vector3D(0, 39, 0);
                Vector3D direction = (*destination) - shiftedPos;
                direction.normalize();
                shift = Vector3D(direction.x * dudeSpeed, direction.y * dudeSpeed, 0);

                Vector3D tmp = shiftedPos + shift;


                if (CollisionCircleCircle(tmp.x, tmp.y, 1.f, destination->x, destination->y, 1.f))
                {
                    ++pathIndex;
                }
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

    }


    Vector3D newPos = pos + shift;

    if (!Actor::isColiding(newPos, &shift, map))
    {
        pos = newPos;
    }
    else
    {
        newPos = pos + shift;

        //printf("%f %f\n", shift.x, shift.y);

        if (!Actor::isColiding(newPos, nullptr, map))
        {
            pos = newPos;
        }
        else
        {
            //printf("%f %f\n", shift.x, shift.y);
        }
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

}
