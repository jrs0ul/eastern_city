#ifndef ITEM_DATA_H
#define ITEM_DATA_H

struct ItemData
{
    int         hungerDecrease;
    bool        isWearable;
    bool        isConsumable;
    bool        isWeapon;
    int         coldDecrease;
    float       spoilageSpeed;
    float       clothingQualityIncrease;
    char        name[256];
    int         ammoItemIndex;
    int         imageIndex;
    int         hpUp;

    ItemData()
    : hungerDecrease(0)
    , isWearable(false)
    , isConsumable(false)
    , isWeapon(false)
    , coldDecrease(0)
    , spoilageSpeed(0.f)
    , clothingQualityIncrease(0.f)
    , ammoItemIndex(-1)
    , imageIndex(0)
    , hpUp(0)
    {
    }
};


#endif //ITEM_DATA_H
