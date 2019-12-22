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

        if (isColiding(pos + shift, map))
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

        if (isColiding(pos + shift, map))
        {
            shift.x = 0.f;
        }
    }


    Vector3D newPos = pos + shift;

    if (!isColiding(newPos, map))
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


    int replacableItemIndex = -1;

    for (unsigned i = 0; i < items.count(); ++i)
    {
        if (items[i].isRemoved())
        {
            replacableItemIndex = i;
            break;
        }
    }


    for (int i = 0; i < map.getItemCount(); ++i)
    {
        ItemInstance* itm = map.getItem(i);

        bool noMorePlaceInBag = ((int)items.count() >= maxItems) ? (replacableItemIndex == -1 ?  true : false) : false;
        
        if (itm->isRemoved() || noMorePlaceInBag)
        {
            continue;
        }

        if (CollisionCircleCircle(shiftedPos.x, shiftedPos.y, 16, itm->getPosition()->x, itm->getPosition()->y, 8))
        {

            if (replacableItemIndex != -1)
            {
                items[replacableItemIndex] = *itm;
            }
            else if ((int)items.count() < maxItems)
            {
                ItemInstance itemToAdd = *itm;
                items.add(itemToAdd);
            }

            
            itm->setAsRemoved();
        }
    }

}

void Dude::draw(float posX, float posY,
                PicsContainer& pics, bool debugInfo)
{
    int frames[][4] = {{0, 1, 2, 1}, {3, 4, 5, 4}, {6, 7, 8, 7}};

    int frame = frames[animationSubset][animationFrame];
    pics.draw(pics.findByName("pics/dude.tga"), 
              pos.x + posX, pos.y + posY,
              frame, true, (isFlipedX)? -1.f: 1.f, 1.f);

    if (debugInfo)
    {
        pics.draw(-1, pos.x + posX, pos.y + posY + 39, 0, true, 32, 32, 0.f, COLOR(1,0,0,0.5f), COLOR(1,0,0,0.5f));
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

bool Dude::isColiding(Vector3D newPos, GameMap& map)
{

    newPos = newPos + Vector3D(0, 39, 0);

    int topX = newPos.x - 16;
    int topY = newPos.y - 16;
    int bottomX = newPos.x + 16;
    int bottomY = newPos.y + 16;

    bool colides = false;

    int fromY = topY / 32;
    int toY = bottomY / 32;
    int fromX = topX / 32;
    int toX = bottomX / 32;

    for (int i = fromY; i <= toY; ++i)
    {
        for (int j = fromX; j <= toX; ++j)
        {
            int res = map.canTraverse(j, i);

            if (res == 0)
            {
                colides = CollisionCircleRectangle(newPos.x, newPos.y, 15.f, j * 32, i * 32, 32, 32);

                if (colides)
                {
                    return true;
                }

            }
            else if (res == -1)
            {
                return true;
            }
        }
    }


    return false;

}
