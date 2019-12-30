#include "GameMap.h"
#include "../Xml.h"
#include <cwchar>


GameMap::GameMap()
{
    collision = nullptr;
}

void GameMap::create()
{
    width = 26;
    height = 15;

    collision = (bool*)malloc(sizeof(bool) * width * height);
    for (int i = 0; i < (int)height; ++i)
    {
        for(int j = 0; j < (int)width; ++j)
        {
            collision[width * i + j] = (rand() % 10 < 2 && j > 4);
        }
    }
}

void GameMap::destroy()
{
    items.destroy();
    assets.destroy();
    regions.destroy();
    entries.destroy();
    
    for (unsigned i = 0; i < containers.count(); ++i)
    {
        containers[i].destroy();
    }

    containers.destroy();

    if (collision)
    {
        free(collision);
    }
}

#ifdef __ANDROID__
void GameMap::load(const char* file,  AAssetManager* assman, GlobalItemList* worldItems , Room* room)
#else
void GameMap::load(const char* file, GlobalItemList* worldItems, Room* room)
#endif
{
    destroy();

    Xml mapfile;
    printf("loading file %s\n", file);
#ifdef __ANDROID__
    mapfile.load(file, assman);
#else
    mapfile.load(file);
#endif
    printf("loaded.\n");
    XmlNode* collisionNode = mapfile.root.getNode(L"Collision");

    width = 0;
    height = 0;
    char buffer[100];

    if (collisionNode)
    {
        for (unsigned i = 0; i < collisionNode->attributeCount(); ++i)
        {
            XmlAttribute* at = collisionNode->getAttribute(i);


            sprintf(buffer, "%ls", at->getName());

            if (strcmp(buffer, "width") == 0)
            {
                sprintf(buffer, "%ls", at->getValue());
                width = atoi(buffer);
            }
            else if (strcmp(buffer, "height") == 0)
            {
                sprintf(buffer, "%ls", at->getValue());
                height = atoi(buffer);
            }
        }

        printf("width: %d height:%d\n", width, height);

        collision = (bool*)malloc(sizeof(bool) * width * height);
        memset(collision, 0, sizeof(bool) * width * height);

        printf("children count %lu\n", collisionNode->childrenCount());

        for (unsigned long i = 0; i < collisionNode->childrenCount(); ++i)
        {
            if (i >= height)
            {
                continue;
            }


            XmlNode* row = collisionNode->getNode(i);

            if (row)
            {
                for (unsigned long j = 0; j < row->childrenCount(); ++j)
                {
                    if (j >= width)
                    {
                        continue;
                    }

                    XmlNode* cell = row->getNode(j);
                    if (cell)
                    {
                        char buffer[100];
                        sprintf(buffer, "%ls", cell->getValue());
                        collision[i * width + j] = atoi(buffer);
                    }
                }
            }
        }

    }

    XmlNode* temperatureNode = mapfile.root.getNode(L"Temperature");

    if (temperatureNode)
    {
        char buffer[100];
        sprintf(buffer, "%ls", temperatureNode->getValue());
        temperature = atoi(buffer);
    }


    XmlNode* assetNode = mapfile.root.getNode(L"Assets");

    if (assetNode)
    {
        for (unsigned i = 0; i < assetNode->childrenCount(); ++i)
        {
            XmlNode* asset = assetNode->getNode(i);

            if (asset)
            { 
                Asset ass;
                ass.spriteIndex = 0;
                ass.containerIndex = -1;

                for (unsigned j = 0; j < asset->attributeCount(); ++j)
                {
                    XmlAttribute* at = asset->getAttribute(j);

                    char buffer[100];

                    sprintf(buffer, "%ls", at->getName());

                    if (strcmp(buffer, "x") == 0)
                    {
                        sprintf(buffer, "%ls", at->getValue());
                        ass.pos.x = atof(buffer);
                    }
                    else if (strcmp(buffer, "y") == 0)
                    {
                        sprintf(buffer, "%ls", at->getValue());
                        ass.pos.y = atof(buffer);
                    }
                    else if (strcmp(buffer, "index") == 0)
                    {
                        sprintf(buffer, "%ls", at->getValue());
                        ass.spriteIndex = atoi(buffer);

                    }
                    else if (strcmp(buffer, "containerIndex") == 0)
                    {
                        sprintf(buffer, "%ls", at->getValue());
                        ass.containerIndex = atoi(buffer);

                    }


                }

                ass.pos.z = 0;

                sprintf(ass.name, "%ls", asset->getValue());

                assets.add(ass);

                printf("x %f y %f %s\n", ass.pos.x, ass.pos.y, ass.name);

            }

        }
    }

    XmlNode* entriesNode = mapfile.root.getNode(L"Entries");
    
    if (entriesNode)
    {
        for (unsigned i = 0; i < entriesNode->childrenCount(); ++i)
        {
            XmlNode* entryNode = entriesNode->getNode(i);

            if (entryNode)
            {
                Vector3D entry(0,0,0);

                for (unsigned j = 0; j < entryNode->attributeCount(); ++j)
                {
                    XmlAttribute* at = entryNode->getAttribute(j);

                    char buffer[100];

                    sprintf(buffer, "%ls", at->getName());

                    if (strcmp(buffer, "x") == 0)
                    {
                        sprintf(buffer, "%ls", at->getValue());
                        entry.x = atof(buffer);
                    }
                    else if (strcmp(buffer, "y") == 0)
                    {
                        sprintf(buffer, "%ls", at->getValue());
                        entry.y = atof(buffer);
                    }
                }

                entry.z = 0;
                printf("entry %f %f\n", entry.x, entry.y);
                entries.add(entry);
            }
        }//for
    }

    XmlNode* regionNode = mapfile.root.getNode(L"Regions");

    if (regionNode)
    {
        for (unsigned i = 0; i < regionNode->childrenCount(); ++i)
        {
            XmlNode* reg = regionNode->getNode(i);
            Region region;
            

            sprintf(region.name, "%ls", reg->getValue());
            
            char buffer[100];

            for (unsigned j = 0; j < reg->attributeCount(); ++j)
            {
                XmlAttribute* ra = reg->getAttribute(j);

                sprintf(buffer, "%ls", ra->getName());

                if (strcmp(buffer, "x") == 0)
                {
                    sprintf(buffer, "%ls", ra->getValue());
                    region.pos.x = atof(buffer);
                }
                else if (strcmp(buffer, "y") == 0)
                {
                    sprintf(buffer, "%ls", ra->getValue());
                    region.pos.y = atof(buffer);
                }
                else if (strcmp(buffer, "width") == 0)
                {
                    sprintf(buffer, "%ls", ra->getValue());
                    region.size.x = atof(buffer);
                }
                else if (strcmp(buffer, "height") == 0)
                {
                    sprintf(buffer, "%ls", ra->getValue());
                    region.size.y = atof(buffer);
                }
                else if (strcmp(buffer, "entry") == 0)
                {
                    sprintf(buffer, "%ls", ra->getValue());
                    region.entryIndex = atoi(buffer);
                }



            }

            printf("region: %f %f %f %f %s %u\n", region.pos.x, region.pos.y, region.size.x, region.size.y, region.name, region.entryIndex);
            region.pos.z = 0;
            region.size.z = 0;

            regions.add(region);

        }

    }


    XmlNode* containersNode = mapfile.root.getNode(L"Containers");

    if (containersNode)
    {
        for (unsigned i = 0; i < containersNode->childrenCount(); ++i)
        {
            XmlNode* containerNode = containersNode->getNode(i);

            ItemContainer ic;

            if (containerNode)
            {
                for (unsigned j = 0; j < containerNode->attributeCount(); ++j)
                {
                    XmlAttribute* att = containerNode->getAttribute(j);
                    
                    if (att)
                    {
                        sprintf(buffer, "%ls", att->getName());

                        if (strcmp("slotCount", buffer) == 0)
                        {
                            sprintf(buffer, "%ls", att->getValue());
                            ic.init(atoi(buffer), 3);
                        }

                    }
                }

                for (unsigned j = 0; j < containerNode->childrenCount(); ++j)
                {
                    XmlNode* itemNode = containerNode->getNode(j);
                    

                    if (itemNode)
                    {
                        for (unsigned k = 0; k < itemNode->attributeCount(); ++k)
                        {
                            XmlAttribute* itemAttribute = itemNode->getAttribute(k);

                            if (itemAttribute)
                            {
                                sprintf(buffer, "%ls", itemAttribute->getName());

                                if (strcmp("index", buffer) == 0)
                                {
                                    sprintf(buffer, "%ls", itemAttribute->getValue());
                                    ItemInstance itm;
                                    itm.init(Vector3D(0, 0, 0), atoi(buffer));
                                    ic.addItem(itm);
                                }

                            }
                        }
                    }
                }

                containers.add(ic);
            }
        }
    }


    if (worldItems)
    {
        printf("there should be %d items in this map\n",  worldItems->getItemCountInMap(file));
        for (unsigned i = 0; i < worldItems->getItemCountInMap(file); ++i)
        {
            ItemInstance* itm = worldItems->get(file,i);

            if (itm)
            {
                items.add(itm);
            }
        }
    }

    if (room)
    {
        for (unsigned i = 0; i < room->getItemCount(); ++i)
        {
            ItemInstance* itm = room->getItem(i);

            if (itm)
            {
                items.add(itm);
            }
        }

        for (unsigned i = 0; i < room->getAssetCount(); ++i)
        {
            Asset ass = *(room->getAsset(i));
            assets.add(ass);
        }

        for (unsigned i = 0; i < room->getAdditionalCollisionCount(); ++i)
        {
            CollisionTile* tile = room->getAdditionalCollisionTile(i);

            if (tile && tile->x < width && tile->y < height)
            {
                collision[tile->y * width + tile->x] = tile->collides;
            }
        }

        for (unsigned i = 0; i < room->getAdditionalRegionsCount(); ++i)
        {
            Region* reg = room->getAdditionalRegion(i);

            if (reg)
            {
                Region r = *reg;
                regions.add(r);
            }
        }

        for (unsigned i = 0; i < room->getAdditionalEntriesCount(); ++i)
        {
            Vector3D* e = room->getAdditionalEntry(i);

            if (e)
            {
                Vector3D entr = *e;
                entries.add(entr);
            }

        }
    }

    mapfile.destroy();
    printf("done.\n");
}

void GameMap::save(const char* file)
{
    if (!collision)
    {
        return;
    }

    Xml mapfile;

    XmlNode collisionNode;
    collisionNode.setName(L"Collision");
    wchar_t buf[100];
    swprintf(buf, 100, L"%u", width);
    collisionNode.addAtribute(L"width", buf);
    swprintf(buf, 100, L"%u", height);
    collisionNode.addAtribute(L"height", buf);

    for (unsigned i = 0; i < height; ++i)
    {
        XmlNode row;
        row.setName(L"tr");
        
        for (unsigned j = 0; j < width; ++j)
        {
            XmlNode cell;
            cell.setName(L"td");
            int isColiding = collision[i * width + j];
            swprintf(buf, 100, L"%d", isColiding);
            cell.setValue(buf);
            row.addChild(cell);
        }

        collisionNode.addChild(row);
    }

    XmlNode temperatureNode;
    temperatureNode.setName(L"Temperature");
    swprintf(buf, 100, L"%d", temperature);
    temperatureNode.setValue(buf);

    XmlNode assetNode;
    assetNode.setName(L"Assets");

    for (unsigned i = 0; i < assets.count(); ++i)
    {
        XmlNode ass;
        ass.setName(L"Asset");
        swprintf(buf, 100, L"%s", assets[i].name);
        ass.setValue(buf);
        swprintf(buf, 100, L"%f", assets[i].pos.x);
        ass.addAtribute(L"x", buf);
        swprintf(buf, 100, L"%f", assets[i].pos.y);
        ass.addAtribute(L"y", buf);

        if (assets[i].spriteIndex > 0)
        {
            swprintf(buf, 100, L"%u", assets[i].spriteIndex);
            ass.addAtribute(L"index", buf);
        }

        assetNode.addChild(ass);
    }

    XmlNode entriesNode;
    entriesNode.setName(L"Entries");
    for (unsigned int i = 0; i < entries.count(); ++i)
    {
        XmlNode entryNode;
        entryNode.setName(L"Entry");
        swprintf(buf, 100, L"%f", entries[i].x);
        entryNode.addAtribute(L"x", buf);
        swprintf(buf, 100, L"%f", entries[i].y);
        entryNode.addAtribute(L"y", buf);
        entriesNode.addChild(entryNode);
    }

    XmlNode regionNode;
    regionNode.setName(L"Regions");

    for (unsigned i = 0; i < regions.count(); ++i)
    {
        XmlNode reg;
        reg.setName(L"Region");
        swprintf(buf, 100, L"%f", regions[i].pos.x);
        reg.addAtribute(L"x", buf);
        swprintf(buf, 100, L"%f", regions[i].pos.y);
        reg.addAtribute(L"y", buf);
        swprintf(buf, 100, L"%f", regions[i].size.x);
        reg.addAtribute(L"width", buf);
        swprintf(buf, 100, L"%f", regions[i].size.y);
        reg.addAtribute(L"height", buf);
        swprintf(buf, 100, L"%u", regions[i].entryIndex);
        reg.addAtribute(L"entry", buf);
        swprintf(buf, 100, L"%s", regions[i].name);
        reg.setValue(buf);

        regionNode.addChild(reg);


    }

    mapfile.root.addChild(collisionNode);
    mapfile.root.addChild(temperatureNode);
    mapfile.root.addChild(assetNode);
    mapfile.root.addChild(entriesNode);
    mapfile.root.addChild(regionNode);

    mapfile.write(file);
}

void GameMap::draw(float posX, float posY, PicsContainer& pics, ItemDatabase& itemDb, bool isDebug)
{

    for (unsigned i = 0; i < assets.count(); ++i)
    {
        pics.draw(pics.findByName(assets[i].name), 
                                  assets[i].pos.x + posX, 
                                  assets[i].pos.y + posY,
                                  assets[i].spriteIndex);
    }

    for (unsigned i = 0; i < items.count(); ++i)
    {
        if (items[i]->isRemoved())
        {
            continue;
        }

        ItemData* data = itemDb.get(items[i]->getIndex());
        pics.draw(4, items[i]->getPosition()->x + posX, items[i]->getPosition()->y + posY, data->imageIndex, true);
    }

    if (isDebug)
    {
        for (int i = 0; i < (int)height; ++i)
        {
            for(int j = 0; j < (int)width; ++j)
            {
                if (collision[width * i + j])
                {
                    pics.draw(-1, j * 32 + posX, i * 32 + posY, 0, false, 32, 32, 0.f, COLOR(1, 0,0, 0.5f), COLOR(1,0,0, 0.5f));
                }
            }
        }

        for (int i = 0; i < (int)regions.count(); ++i)
        {
            pics.draw(-1, regions[i].pos.x + posX, regions[i].pos.y + posY, 0, false,
                          regions[i].size.x, regions[i].size.y,
                          0.f, COLOR(0.f, 1.f, 0.f, 0.5f), COLOR(0.f, 1.f, 0, 0.5f));
        }

        for (int i = 0; i < (int)entries.count(); ++i)
        {
            pics.draw(-1,entries[i].x + posX, entries[i].y + posY, 0, true,
                          8, 8,
                          0.f, COLOR(0.f, 0.f, 1.f, 0.5f), COLOR(0.f, 0.f, 1.f, 0.5f));
        }

    }

}

void GameMap::addItem(ItemInstance* item)
{
    items.add(item);
}

int GameMap::canTraverse(int x, int y)
{

    if (!collision)
    {
        return -1;
    }

    if (x <= 0 || y <= 0)
    {
        return -1;
    }

    if (x >= (int)width || y >= (int)height)
    {
        return -1;
    }


    return !collision[y * width + x];
}

Asset* GameMap::getClickedAsset(float mapOffsetX, float mapOffsetY,
                                PicsContainer& pics,
                                int x, int y)
{
    const float currentX = x - mapOffsetX;
    const float currentY = y - mapOffsetY;

    if (!assets.count())
    {
        return nullptr;
    }

    for (int i = (int)assets.count() - 1; i >= 0; --i)
    {
        Asset* asset = &assets[i];

        PicData* data = pics.getInfo(pics.findByName(asset->name));


        if (data)
        {
            int spriteHeight = data->height;
            int spriteWidth = data->width;

            if (data->sprites.count())
            {
                Sprite* sprite = &(data->sprites[asset->spriteIndex]);
                spriteHeight = sprite->height; 
                spriteWidth = sprite->width;
            }
        
            if (currentX > asset->pos.x 
                && currentX < asset->pos.x + spriteWidth
                && currentY > asset->pos.y
                && currentY < asset->pos.y + spriteHeight)
            {
                return asset;
            }
        }
    }

    return nullptr;
}

void GameMap::setCollision(int x, int y, bool bColide)
{
    if (!collision)
    {
        return;
    }

    if (x < 0 || y < 0)
    {
        return;
    }

    if (x >= (int)width || y >= (int)width)
    {
        return;
    }

     collision[y * width + x] = bColide;
}

Vector3D* GameMap::getPlayerPos(unsigned index)
{
    if (index < entries.count())
    {
        return &entries[index];
    }

    printf("Error-> Entry count: %lu, index:%u\n", entries.count(), index);

    return nullptr;
}


ItemContainer* GameMap::getItemContainer(unsigned index)
{
    if (index < containers.count())
    {
        return &containers[index];
    }

    return nullptr;
}

