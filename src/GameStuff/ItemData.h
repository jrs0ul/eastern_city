#ifndef ITEM_DATA_H
#define ITEM_DATA_H

struct ItemData
{
    int         hungerDecrease;
    bool        isWearable;
    bool        isConsumable;
    bool        isWeapon;
    bool        isLoadable;
    int         loadCapacity;
    int         coldDecrease;
    int         sleepynessDecrease;
    int         attack;
    float       spoilageSpeed;
    float       clothingQualityIncrease;
    char        name[256];
    int         ammoItemIndex;
    int         imageIndex;
    int         secondImageIndex;
    int         hpUp;

    ItemData()
    : hungerDecrease(0)
    , isWearable(false)
    , isConsumable(false)
    , isWeapon(false)
    , isLoadable(false)
    , loadCapacity(0)
    , coldDecrease(0)
    , sleepynessDecrease(0)
    , attack(0)
    , spoilageSpeed(0.f)
    , clothingQualityIncrease(0.f)
    , ammoItemIndex(-1)
    , imageIndex(0)
    , secondImageIndex(-1)
    , hpUp(0)
    {
    }
};


#endif //ITEM_DATA_H
