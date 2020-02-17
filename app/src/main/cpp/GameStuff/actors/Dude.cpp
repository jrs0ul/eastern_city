#include "Dude.h"
#include "../Statistics.h"
#include "../FurnitureData.h"
#include "../../TextureLoader.h"
#include "../../MathTools.h"
#include "../../audio/SoundSystem.h"
#include "../../gui/Text.h"
#include <cmath>

void Dude::draw(float offsetX, float offsetY,
                         int scale,
                         PicsContainer& pics, 
                         bool debugInfo)
{
    Actor::draw(offsetX, offsetY, scale, pics, debugInfo);

    if (debugInfo && doAttack)
    {
        pics.draw(-1, 
                  attackBoxPos.x * scale + offsetX,
                  attackBoxPos.y * scale + offsetY,
                  0,
                  false,
                  attackBoxSize.x * scale,
                  attackBoxSize.y * scale,
                  0.f,
                  COLOR(0.5, 0, 0, 0.8f),
                  COLOR(0.5, 0, 0, 0.8f));
    }
}



void Dude::init(Vector3D& position, int ScreenWidth, int ScreenHeight, int scale)
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
    itemBag.setPosition(Vector3D(100 * scale, ScreenHeight - (32 + 3) * scale, 0));
    equipedItems.init(2, 2);
    equipedItems.setPosition(Vector3D(ScreenWidth - (3 * 32) * scale, ScreenHeight - (32 + 3) * scale, 0));
    satiationProgress = 0.f;
    freezingProgress = 0.f;
    satiation = 100;
    wakefullness = 100;
    warmth = 100.f;
    health = 100.f;
    timeAwake = 0.f;
    collisionBodyOffset = Vector3D(0.f, 39.f, 0.f);
    collisionBodyRadius = 16.f;
    pictureIndex = NAKED_IDLE;
    equipedWeaponInLastFrame = -1;
    doAttack = false;

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


    const float axeOffsetY = -10;

    FrameSet axe_up;
    axe_up.offsetY = axeOffsetY;
    axe_up.frames.add(0);
    axe_up.frames.add(1);
    axe_up.frames.add(2);
    axe_up.frames.add(1);

    FrameSet axe_down;
    axe_down.offsetY = axeOffsetY;
    axe_down.frames.add(5);
    axe_down.frames.add(6);
    axe_down.frames.add(7);
    axe_down.frames.add(6);

    FrameSet axe_side;
    axe_side.offsetY = axeOffsetY;
    axe_side.frames.add(10);
    axe_side.frames.add(11);
    axe_side.frames.add(12);
    axe_side.frames.add(11);

    FrameSet axe_attack_up;
    axe_attack_up.offsetY = axeOffsetY;
    axe_attack_up.frames.add(1);
    axe_attack_up.frames.add(3);
    axe_attack_up.frames.add(4);

    FrameSet axe_attack_down;
    axe_attack_down.offsetY = axeOffsetY;
    axe_attack_down.frames.add(6);
    axe_attack_down.frames.add(8);
    axe_attack_down.frames.add(9);

    FrameSet axe_attack_side;
    axe_attack_side.offsetY = axeOffsetY;
    axe_attack_side.frames.add(11);
    axe_attack_side.frames.add(13);
    axe_attack_side.frames.add(14);


    animations.add(up);
    animations.add(down);
    animations.add(side);
    animations.add(sleep);
    animations.add(axe_up);
    animations.add(axe_down);
    animations.add(axe_side);
    animations.add(axe_attack_up);
    animations.add(axe_attack_down);
    animations.add(axe_attack_side);
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
                  Room* currentRoom,
                  ActorContainer& actors,
                  float darkness,
                  ItemDatabase& itemdb,
                  CraftingRecipes& recipes,
                  FurnitureDatabase& furnitureDb,
                  FindPath& path)
{

    if (equipedWeaponInLastFrame != isWeaponEquiped() && isWeaponEquiped() == -1)
    {
        onWeaponUnequip();
    }

    const bool axeEquiped = (isWeaponEquiped() == 19 && !sleeping);
    equipedWeaponInLastFrame = isWeaponEquiped();

    if (isDamaged)
    {
        damageProgress -= deltaTime;
        
        if (damageProgress <= 0.f)
        {
            isDamaged = false;
        }
    }

    int equipedItemIndex = isClothesEquiped();
    bool coatEquiped = (equipedItemIndex == 2 || equipedItemIndex == 16);
    
    if (isWeaponEquiped() == 12 && !sleeping)
    {
        pictureIndex = (coatEquiped) ? COAT_FLASHLIGHT : NAKED_FLASHLIGHT;
    }
    else if (axeEquiped)
    {
        pictureIndex = (coatEquiped) ? COAT_AXE : NAKED_AXE;
    }
    else if (sleeping)
    {
        pictureIndex = (coatEquiped) ? COAT_SLEEP : NAKED_SLEEP;
    }
    else
    {
        pictureIndex = (coatEquiped) ? COAT_IDLE : NAKED_IDLE;
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
        melleeAttacks(actors, map, currentRoom, itemdb, recipes, furnitureDb, Keys, deltaTime);
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

void Dude::drawInventory(PicsContainer& pics,
                         ItemDatabase& itemDb,
                         ItemInstance* selectedItem,
                         int scale)
{
    itemBag.draw(pics, itemDb, selectedItem, scale, false);
    equipedItems.draw(pics, itemDb, selectedItem, scale, true);
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

        if (data->hungerDecrease > 0)
        {
            SoundSystem::getInstance()->playsound(0);
        }

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

        wakefullness += data->sleepynessDecrease;

        if (wakefullness > 100)
        {
            wakefullness = 100;
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
            onWeaponUnequip();
        }

        if (equipedItems.addItem(*item, 1))
        {
            item->setAsRemoved();
            Statistics::getInstance()->increaseEquipedTimes();

            onWeaponEquip();
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

bool Dude::checkInventoryInput(int scale,
                               float deltaTime,
                               TouchData& touches, 
                               ItemInstance** selectedItem, 
                               bool& itemSelected,
                               bool& clickedOnInventory,
                               Vector3D& itemPos,
                               void** doubleClickCallbackData)
{
    itemBag.setActive(true);
    unsigned  itemsBefore = itemBag.getActualItemCount();

    bool clickedOnInventoryItems = false;
    bool clickedOnEquipedItems = false;

    bool res = itemBag.checkInput(scale, deltaTime, touches, selectedItem, itemSelected, clickedOnInventoryItems, itemPos, doubleClickCallbackData);

   
    unsigned itemsAfter = itemBag.getActualItemCount();
    
    if (itemsBefore < itemsAfter)
    {
        Statistics::getInstance()->increaseItemAddition();
    }

    equipedItems.setActive(true);
    itemsBefore = equipedItems.getActualItemCount();
    bool isWeaponEquipedBefore = (isWeaponEquiped() != -1);
     

    res = equipedItems.checkInput(scale, deltaTime, touches, selectedItem, itemSelected, clickedOnEquipedItems, itemPos, nullptr);
    itemsAfter = equipedItems.getActualItemCount();
    bool isWeaponEquipedNow = (isWeaponEquiped() != -1);

        
    if (isWeaponEquipedBefore && !isWeaponEquipedNow)
    {
        onWeaponUnequip();
    }

    if (!isWeaponEquipedBefore && isWeaponEquipedNow)
    {
        onWeaponEquip();
    }
    
    if (itemsBefore < itemsAfter)
    {
        Statistics::getInstance()->increaseEquipedTimes();
    }

    clickedOnInventory = clickedOnInventoryItems | clickedOnEquipedItems;


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

void Dude::activateMeleeAttack()
{
    if (isWeaponEquiped() == 19 && !sleeping && !doAttack)
    {
        doAttack = true;
        animationFrame = 0;
        animationProgress = 0.f;

        const float attackBoxWidth = 48.f;

        switch(animationSubset)
        {
            case 6: {
                animationSubset = 9;
                
                if (isFlipedX)
                {
                    attackBoxPos = Vector3D(pos.x + 8, pos.y - 16, 0);
                    attackBoxSize = Vector3D(attackBoxWidth, 64, 0);
                }
                else
                {
                    attackBoxPos = Vector3D(pos.x - attackBoxWidth - 8, pos.y - 16, 0);
                    attackBoxSize = Vector3D(attackBoxWidth, 64, 0);
                }

            } break;
            case 5: 
            {
                animationSubset = 8;
                attackBoxPos = Vector3D(pos.x - 24, pos.y - 48, 0);
                attackBoxSize = Vector3D(attackBoxWidth, 64, 0);
            } break;
            case 4:{
                animationSubset = 7;
                attackBoxPos = Vector3D(pos.x - 24, pos.y + 8, 0);
                attackBoxSize = Vector3D(attackBoxWidth, 60, 0);
            }break;
        }

    }
}

void Dude::goToSleep()
{
    sleeping = true; 
    sleepAnimationDone = false;
    animationSubset = 3; 
    animationFrame = 0;
    timeAwake = 0.f;
}

void Dude::wearWeapon(float damage)
{
    ItemInstance* wep = equipedItems.getItem(1);

    if (wep && !wep->isRemoved())
    {
        auto currentQuality = wep->getQuality();
        wep->setQuality(currentQuality + damage);

        if (currentQuality + damage <= 0)
        {
            wep->setAsRemoved();
        }
    }
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
        wakefullness += deltaTime * 0.6f;

        if (wakefullness > 100)
        {
            wakefullness = 100;
        }
           
        return;
    }


    timeAwake += deltaTime;
    //TODO: use const file
    const float dayLength = 480.f;
    
    if (timeAwake > (dayLength / 24) * 4)
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
    freezingProgress += deltaTime * 1.0f;

    if (freezingProgress >= 1.f)
    {
        int hitCount = (int)freezingProgress;

        int clothingTemperature = 0;

        if (isClothesEquiped() != -1)
        {
            clothingTemperature = itemdb.get(equipedItems.getItem(0)->getIndex())->coldDecrease;
        }

        if (itemBag.hasItem(21) != -1 && sleeping)
        {
            clothingTemperature = itemdb.get(21)->coldDecrease;
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
    satiationProgress += deltaTime * 0.5f;

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
    if (doAttack)
    {
        return;
    }

    const bool axeEquiped = (isWeaponEquiped() == 19 && !sleeping);
    
    float dudeSpeed = 1.8f;

    const bool followPath = pathIndex < path.getPathLength();

    if (!doAttack)
    {
        if ((useKeys || followPath) && walkAnimationDone)
        {
            playWalkAnimation = true;
            walkAnimationDone = false;
        }
        else if (walkAnimationDone)
        {
            animationFrame = 1;
        }
    }

    Vector3D shift(0, 0, 0);

    if (!doAttack)
    {
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
    }

    

    if (fabsf(shift.y) > fabsf(shift.x))
    {
        if (shift.y < 0.f)
        {
            animationSubset = (axeEquiped) ? 5 : 1;
        }
        else if (shift.y > 0.f)
        {
            
            animationSubset = (axeEquiped) ? 4 : 0;
        }

    }

    if (fabsf(shift.y) < fabsf(shift.x))
    {
        if (shift.x < 0.f)
        {
            animationSubset = (axeEquiped) ? 6 : 2;;
            isFlipedX = false;
        }
        else if (shift.x > 0.f)
        {
            animationSubset = (axeEquiped) ? 6 : 2;
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


        if (!Actor::isColiding(newPos, nullptr, map))
        {
            pos = newPos;
        }
    }

    //---

    if (!doAttack)
    {
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
}

void Dude::melleeAttacks(ActorContainer& actors,
        GameMap& map,
        Room* currentRoom,
        ItemDatabase& itemdb,
        CraftingRecipes& recipes,
        FurnitureDatabase& furnitureDb,
        unsigned char* Keys,
        float deltaTime
        )
{

    if (Keys[4])
    {
        activateMeleeAttack();
    }

    if (doAttack)
    {
        animationProgress += deltaTime * 5.f;

        if (animationProgress >= 1.f)
        {
            ++animationFrame;
            animationProgress = 0.f;

            if (animationFrame > 2)
            {
                animationFrame = 0;
                doAttack = false;
                //----------hit rats
                for (unsigned i = 0; i < actors.getActorCount(); ++i)
                {
                    Actor* actor = actors.getActor(i);

                    if (actor != this 
                            && !actor->isDead 
                            && CollisionCircleRectangle(
                                actor->pos.x + actor->collisionBodyOffset.x, 
                                actor->pos.y + actor->collisionBodyOffset.y, 
                                actor->collisionBodyRadius,
                                attackBoxPos.x, attackBoxPos.y,
                                attackBoxSize.x, attackBoxSize.y)
                            && actor->getType() == 1)
                    {
                        actor->kill();
                        currentRoom->addItem(Vector3D(actor->pos.x, actor->pos.y, 0), 1);
                        map.addItem(currentRoom->getItem(currentRoom->getItemCount() - 1));
                        wearWeapon(-8.f);
                    }
                }
                //-----------------
                DArray<Furniture*> furniture;
                if (map.getFurnitureInBBox(furniture, attackBoxPos, attackBoxSize))
                {
                    for (unsigned i = 0; i < furniture.count(); ++i)
                    {
                        Furniture* fur = furniture[i];
                        fur->hp -= 10;
                        FurnitureData* fd = furnitureDb.getFurniture(fur->furnitureDbIndex);
                        
                        if (fd)
                        {
                            wearWeapon(fd->damageToAxe * -1.f);
                        }

                        if (fur->hp <= 0)
                        {
                            destroyFurniture(fur, currentRoom, map, recipes);
                        }
                    }
                }

                furniture.destroy();
                //------------------

                switch(animationSubset)
                {
                    case 9: animationSubset = 6; break;
                    case 8: animationSubset = 5; break;
                    case 7: animationSubset = 4; break;
                }

            }
        }
    }

}

void Dude::destroyFurniture(Furniture* fur,
                            Room* currentRoom,
                            GameMap& map,
                            CraftingRecipes& recipes)
{
    printf("YOU ATEMPTED TO DESTROY!!!ĄĄ\n");
    printf("Furniture db index %d\n", fur->furnitureDbIndex);

    Recipe* r = recipes.getRecipeByFurnitureIndex(fur->furnitureDbIndex);

    if (r)
    {
        for (unsigned i = 0; i < r->ingredients.count(); ++i)
        {
            printf("items: %d\n", r->ingredients[i].itemIndex);

            for (int j = 0; j < r->ingredients[i].count; ++j)
            {
                currentRoom->addItem(Vector3D(fur->pos.x + rand() % 30 + (fur->collisionBodySize.x / 2),
                            fur->pos.y + fur->collisionBodySize.y, 
                            0),
                        r->ingredients[i].itemIndex);
                map.addItem(currentRoom->getItem(currentRoom->getItemCount() - 1));
            } 
        }

        if (fur->itemContainerIndex != -1)
        {
            printf("Has some items inside\n");
            ItemContainer* ic = map.getItemContainer(fur->itemContainerIndex);
            for (unsigned j = 0; j < ic->getItemCount(); ++j)
            {
                ItemInstance* item = ic->getItem(j);
                printf("items: %d\n", item->getIndex());

                if (!item->isRemoved())
                {
                    currentRoom->addItem(Vector3D(fur->pos.x + rand() % 30 + (fur->collisionBodySize.x / 2),
                                fur->pos.y + fur->collisionBodySize.y, 
                                0),
                            item);
                    map.addItem(currentRoom->getItem(currentRoom->getItemCount() - 1));
                }
            }

        }

        fur->removed = true;

    }

    map.updateFurniturePolygons(currentRoom);
}

void Dude::onWeaponEquip()
{
   // printf("EQUIP\n");
    if (isWeaponEquiped() == 19)
    {
        switch(animationSubset)
        {
            case 0 : animationSubset = 4; break;
            case 1 : animationSubset = 5; break;
            case 2 : animationSubset = 6; break;
        }
    }
}

void Dude::onWeaponUnequip()
{
   // printf("UNEQUIP\n");
    switch(animationSubset)
    {
        case 4 : animationSubset = 0; break;
        case 5 : animationSubset = 1; break;
        case 6 : animationSubset = 2; break;
        case 7 : animationSubset = 0; break;
        case 8 : animationSubset = 1; break;
        case 9 : animationSubset = 2; break;
        default :{}
    }
}
