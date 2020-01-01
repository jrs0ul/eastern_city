#include "ItemInstance.h"


void ItemInstance::init(Vector3D position, int index)
{
    pos = position;
    itemIndex = index;
    removed = false;
    quality = 100.f;
    ammoLoaded = 0;
}

