#include "Room.h"

void Room::addChildRoom(const char* name, unsigned entry, unsigned region)
{
    RoomAndEntry entr;
    entr.room = new Room();
    entr.room->setMapName(name);
    entr.entryIndex = entry;
    entr.region = region;

    childRooms.add(entr);
}

void Room::addChildRoom(Room* room, unsigned entry, unsigned region)
{
    RoomAndEntry entr;
    entr.room = room;
    entr.entryIndex = entry;
    entr.region = region;
    childRooms.add(entr);
}

void Room::addItem(Vector3D pos, int index)
{
    ItemInstance item;
    item.init(pos, index);
    items.add(item);
}

void Room::addItem(Vector3D pos, ItemInstance* item)
{
    ItemInstance newItem = *item;
    newItem.setPosition(pos);
    items.add(newItem);
}

void Room::addItemContainer(unsigned furnitureIndex, ItemContainer& container)
{
    if (furnitureIndex > furniture.count())
    {
        return;
    }

    furniture[furnitureIndex]->itemContainerIndex = itemContainers.count();
    itemContainers.add(container);
}

void Room::addEnemyPosition(Vector3D pos)
{
    enemies.add(pos);
}

void Room::addAsset(Vector3D pos, 
                    const char* name, 
                    unsigned spriteIndex, 
                    bool flipped,
                    bool frontLayer)
{
    Asset ass;
    ass.pos = pos;
    ass.isFlipped = flipped;
    ass.isInFrontLayer = frontLayer;
    strcpy(ass.name, name);
    ass.spriteIndex = spriteIndex;
    assets.add(ass);
}

void Room::addFurniture(Furniture& f)
{
    Furniture* fur = new Furniture();

    fur->isBed = f.isBed;
    fur->removed = f.removed;
    fur->colidedWithHero = f.colidedWithHero;
    fur->pictureIndex = f.pictureIndex;
    fur->spriteIndex = f.spriteIndex;
    fur->collisionBodySize = f.collisionBodySize;
    fur->collisionBodyPos = f.collisionBodyPos;
    fur->furnitureDbIndex = f.furnitureDbIndex;
    fur->hp = f.hp;

    float smallestY = 99999;

    for (unsigned i = 0; i < f.collisionPolygon.points.count(); ++i)
    {
        if (f.collisionPolygon.points[i].y < smallestY)
        {
            smallestY = f.collisionPolygon.points[i].y;
        }

        fur->collisionPolygon.points.add(f.collisionPolygon.points[i]);
        
    }

    fur->pos = f.pos;

    fur->collisionBodyOffset = Vector3D(0, smallestY, 0);

    furniture.add(fur);
}

void Room::addCollisionPolygon(SPolygon& p)
{
    SPolygon pol;
    collisionPolygons.add(pol);
    SPolygon* pp = &collisionPolygons[collisionPolygons.count() - 1];

    for (unsigned i = 0; i < p.points.count(); ++i)
    {
        pp->points.add(p.points[i]);
    }

}

void Room::addDoorHole(float x1, float x2, float height)
{
    doorHoles.add(Vector3D(x1, x2, height));
}

void Room::addRegion(Vector3D pos, Vector3D size)
{
    Region reg;
    reg.pos = pos;
    reg.size = size;
    additionalRegions.add(reg);
}

void Room::addEntry(Vector3D pos)
{
    additionalEntries.add(pos);
}

void  Room::addVerticesAfter(SPolygon& p, unsigned indexToPutVerticesAfter)
{
    AdditionalVertices av;

    for (unsigned i = 0; i < p.points.count(); ++i)
    {
        av.p.points.add(p.points[i]);
    }

    av.index = indexToPutVerticesAfter;

    additionalVertices.add(av);
                
}

void Room::removeItem(unsigned index)
{
    if (index < items.count())
    {
        items[index].setAsRemoved();
    }
}

unsigned Room::getItemCount()
{
    return items.count();
}

unsigned Room::getItemContainerCount()
{
    return itemContainers.count();
}

unsigned Room::getEnemyCount()
{
    return enemies.count();
}

unsigned Room::getChildRoomCount()
{
    return childRooms.count();
}

unsigned Room::getAssetCount()
{
    return assets.count();
}

unsigned Room::getFurnitureCount()
{
    return furniture.count();
}

unsigned Room::getAdditionalRegionsCount()
{
    return additionalRegions.count();
}

unsigned Room::getAdditionalEntriesCount()
{
    return additionalEntries.count();
}

unsigned Room::getCollisionPolygonCount()
{
    return collisionPolygons.count();
}

unsigned Room::getDoorHoleCount()
{
    return doorHoles.count();
}

ItemInstance* Room::getItem(unsigned index)
{
    if (index < items.count())
    {
        return &items[index];
    }

    return nullptr;
}

ItemContainer* Room::getItemContainer(unsigned index)
{
    if (index < itemContainers.count())
    {
        return &itemContainers[index];
    }

    return nullptr;
}

Vector3D* Room::getEnemyPosition(unsigned index)
{
    if (index < enemies.count())
    {
        return &enemies[index];
    }

    return nullptr;
}


RoomAndEntry* Room::getChildRoom(unsigned index)
{
    if (index < childRooms.count())
    {
        return &childRooms[index];
    }

    return nullptr;
}


RoomAndEntry* Room::findChildRoomByRegionIndex(unsigned regionIndex)
{
    for (unsigned i = 0; i < childRooms.count(); ++i)
    {
        if (childRooms[i].region == regionIndex)
        {
            return &childRooms[i];
        }
    }

    return nullptr;
}

Asset* Room::getAsset(unsigned index)
{
    if (index < assets.count())
    {
        return &assets[index];
    }

    return nullptr;
}

Furniture* Room::getFurniture(unsigned index)
{
    if (index < furniture.count())
    {
        return furniture[index];
    }

    return nullptr;
}

Region* Room::getAdditionalRegion(unsigned index)
{
    if (index < additionalRegions.count())
    {
        return &additionalRegions[index];
    }

    return nullptr;

}

Vector3D* Room::getAdditionalEntry(unsigned index)
{
    if (index < additionalEntries.count())
    {
        return &additionalEntries[index];
    }

    return nullptr;

}

SPolygon* Room::getCollisionPolygon(unsigned index)
{
    if (index < collisionPolygons.count())
    {
        return &collisionPolygons[index];
    }

    return nullptr;

}

AdditionalVertices* Room::getAdditionalVertices(unsigned index)
{
    if (index < additionalVertices.count())
    {
        return &additionalVertices[index];
    }

    return nullptr;
}

Vector3D* Room::getDoorHole(unsigned index)
{
    if (index < doorHoles.count())
    {
        return &doorHoles[index];
    }
    
    return nullptr;
}


const char* Room::getMapName()
{
    return mapName;
}

void  Room::setMapName(const char* name)
{
    strcpy(mapName, name);
}


void Room::destroy(Room* parent, int level)
{
    /*for (int i = 0; i < level; ++i)
    {
        printf("-");
    }

    printf("deleting %s [child rooms:%lu assets:%lu; parent: %s]\n", mapName, childRooms.count(), assets.count(), parent->mapName);*/

    for (unsigned i = 0; i < childRooms.count(); ++i)
    {
        if (childRooms[i].room != parent)
        {
            //printf("%p != %p\n", childRooms[i].room, parent);
            childRooms[i].room->destroy(this, level + 1);
            delete childRooms[i].room;
        }
    }

    /*for (int i = 0; i < level; ++i)
    {
        printf("-");
    }

    printf("%s children deleted\n", mapName);*/

    for (unsigned i = 0; i < itemContainers.count(); ++i)
    {
        itemContainers[i].destroy();
    }

    for (unsigned i = 0; i < additionalVertices.count(); ++i)
    {
        additionalVertices[i].p.points.destroy();
    }

    additionalVertices.destroy();

    itemContainers.destroy();

    childRooms.destroy();
    items.destroy();
    enemies.destroy();
    assets.destroy();
    doorHoles.destroy();

    for (unsigned i = 0; i < collisionPolygons.count(); ++i)
    {
        collisionPolygons[i].points.destroy();
    }

    collisionPolygons.destroy();
    
    for (unsigned i = 0; i < furniture.count(); ++i)
    {
        furniture[i]->destroy();
        delete furniture[i];
    }

    furniture.destroy();


    additionalRegions.destroy();
    additionalEntries.destroy();
}


