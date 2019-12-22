#ifndef ITEM_DATA_H
#define ITEM_DATA_H

struct ItemData
{
    int         hungerDecrease;
    bool        isWearable;
    bool        isConsumable;
    int         coldDecrease;
    float       spoilageSpeed;
    char        name[256];
    int         imageIndex;
    int         hpUp;

    ItemData()
    : hungerDecrease(0)
    , isWearable(false)
    , isConsumable(false)
    , coldDecrease(0)
    , spoilageSpeed(0.0f)
    , imageIndex(0)
    , hpUp(0)
    {
    }
};


#endif //ITEM_DATA_H
