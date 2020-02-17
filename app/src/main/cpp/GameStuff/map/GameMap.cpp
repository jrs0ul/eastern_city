#include "GameMap.h"
#include "../Statistics.h"
#include "../../Xml.h"
#include "../../MathTools.h"
#include <cwchar>


GameMap::GameMap()
{
}

void GameMap::destroy()
{
    for (unsigned long i = 0; i < polygons.count(); ++i)
    {
        polygons[i].points.destroy();
    }

    polygons.destroy();

    addititionalNodePoints.destroy();


    items.destroy();
    assets.destroy();
    regions.destroy();
    entries.destroy();
    containers.destroy();
    furniture.destroy();

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

    }
//----------------
    XmlNode* polygonsNode = mapfile.root.getNode(L"Polygons");

    if (polygonsNode)
    {
        for (unsigned long i = 0; i < polygonsNode->childrenCount(); ++i)
        {
            XmlNode* polygonNode = polygonsNode->getNode(i);

            if (polygonNode)
            {
                SPolygon poly;

                for (unsigned long j = 0; j < polygonNode->childrenCount(); ++j)
                {
                    XmlNode* pointNode = polygonNode->getNode(j);
                    
                    if (pointNode)
                    {
                        Vector3D point(0,0,0);

                        for (unsigned long h = 0; h < pointNode->attributeCount(); ++h)
                        {
                            XmlAttribute* at = pointNode->getAttribute(h);

                            sprintf(buffer, "%ls", at->getName());

                            if (strcmp(buffer, "x") == 0)
                            {
                                sprintf(buffer, "%ls", at->getValue());
                                point.x = atof(buffer);
                            }
                            else if (strcmp(buffer, "y") == 0)
                            {
                                sprintf(buffer, "%ls", at->getValue());
                                point.y = atof(buffer);
                            }
                        }

                        poly.points.add(point);
                    }
                }

                polygons.add(poly);
            }
        }
    }
//--------------
    XmlNode* additionalPathPointsNode = mapfile.root.getNode(L"AdditionalPathNodes");

    if (additionalPathPointsNode)
    {
        for (unsigned i = 0; i < additionalPathPointsNode->childrenCount(); ++i)
        {

            XmlNode* pointNode = additionalPathPointsNode->getNode(i);

            if (pointNode)
            {
                Vector3D point(0,0,0);

                for (unsigned long h = 0; h < pointNode->attributeCount(); ++h)
                {
                    XmlAttribute* at = pointNode->getAttribute(h);

                    sprintf(buffer, "%ls", at->getName());

                    if (strcmp(buffer, "x") == 0)
                    {
                        sprintf(buffer, "%ls", at->getValue());
                        point.x = atof(buffer);
                        printf("x:%s\n",buffer);
                    }
                    else if (strcmp(buffer, "y") == 0)
                    {
                        sprintf(buffer, "%ls", at->getValue());
                        point.y = atof(buffer);
                        printf("y:%s\n",buffer);
                    }
                }

                addititionalNodePoints.add(point);
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
                    else if (strcmp(buffer, "flipped") == 0)
                    {
                        sprintf(buffer, "%ls", at->getValue());
                        ass.isFlipped = atoi(buffer);
                    }
                    else if (strcmp(buffer, "front") == 0)
                    {
                        sprintf(buffer, "%ls", at->getValue());
                        ass.isInFrontLayer = atoi(buffer);
                    }
                    else if (strcmp(buffer, "light") == 0)
                    {
                        sprintf(buffer, "%ls", at->getValue());
                        ass.isLight = atoi(buffer);
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


  /*  XmlNode* containersNode = mapfile.root.getNode(L"Containers");

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
    }*/


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

        for (unsigned i = 0; i < room->getCollisionPolygonCount(); ++i)
        {
            SPolygon* p = room->getCollisionPolygon(i);
            SPolygon newPol;
            polygons.add(newPol);
            SPolygon* addedPolygon = &polygons[polygons.count() - 1];

            for (unsigned j = 0; j < p->points.count(); ++j)
            {
                addedPolygon->points.add(p->points[j]);
            }
        }
        //---
        for (unsigned i = 0; i < room->getAdditionalVerticesCount(); ++i)
        {
            AdditionalVertices* av = room->getAdditionalVertices(i);
            
            SPolygon pol;

            for (unsigned j = 0; j <= av->index; ++j)
            {
                pol.points.add(polygons[0].points[j]);
            }

            for (unsigned j = 0; j < av->p.points.count(); ++j)
            {
                pol.points.add(av->p.points[j]);
            }

            for (unsigned j = av->index + 1; j < polygons[0].points.count(); ++j)
            {
                pol.points.add(polygons[0].points[j]);
            }

            polygons[0].points.destroy();

            polygons[0] = pol; // ??
        }
        //--

        for (unsigned i = 0; i < room->getDoorHoleCount(); ++i)
        {
            Vector3D* dh = room->getDoorHole(i);
            createDoorHole(dh->x, dh->y, dh->z);
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

        for (unsigned i = 0; i < room->getItemContainerCount(); ++i)
        {
            ItemContainer* container = room->getItemContainer(i);

            if (container)
            {
                containers.add(container);
            }
        }

        polygonsBeforeFurnitureAdded = polygons.count();


        for (unsigned i = 0; i < room->getFurnitureCount(); ++i)
        {
            Furniture* fur = room->getFurniture(i);

            if (fur && !fur->removed)
            {
                furniture.add(fur);
                SPolygon poly;

                for (unsigned j = 0; j < fur->collisionPolygon.points.count(); ++j)
                {
                    poly.points.add(fur->pos + fur->collisionPolygon.points[j]);
                }

                polygons.add(poly);
            }
        }
    }

    mapfile.destroy();

    Statistics::getInstance()->increaseVisitedRooms();

    printf("done.\n");
}

void GameMap::save(const char* file)
{

    Xml mapfile;

    XmlNode collisionNode;
    collisionNode.setName(L"Collision");
    wchar_t buf[100];
    swprintf(buf, 100, L"%u", width);
    collisionNode.addAtribute(L"width", buf);
    swprintf(buf, 100, L"%u", height);
    collisionNode.addAtribute(L"height", buf);
 
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

void GameMap::draw(float posX, 
                   float posY,
                   int scale,
                   unsigned screenWidth,
                   unsigned screenHeight,
                   PicsContainer& pics, 
                   ItemDatabase& itemDb, 
                   bool isDebug)
{

    for (unsigned i = 0; i < assets.count(); ++i)
    {
        if (assets[i].isInFrontLayer || assets[i].isLight)
        {
            continue;
        }

        unsigned long picIndex = pics.findByName(assets[i].name);

        PicData* data = pics.getInfo(picIndex);

        pics.draw(picIndex, 
                  (assets[i].pos.x + (assets[i].isFlipped ? data->htilew * 2 : 0)) * scale + posX, 
                  assets[i].pos.y * scale + posY,
                  assets[i].spriteIndex,
                  false,
                  (assets[i].isFlipped ? -1.f * scale: 1.f * scale),
                   1.f * scale
                  );
    }
 
    for (unsigned i = 0; i < items.count(); ++i)
    {
        if (items[i]->isRemoved())
        {
            continue;
        }

        ItemData* data = itemDb.get(items[i]->getIndex());
        pics.draw(4, 
                  items[i]->getPosition()->x * scale + posX,
                  items[i]->getPosition()->y * scale + posY,
                  data->imageIndex, true,
                  scale, scale);
    }

    if (isDebug)
    {
        
        for (int i = 0; i < (int)regions.count(); ++i)
        {
            pics.draw(-1, regions[i].pos.x * scale + posX,
                          regions[i].pos.y * scale + posY, 0, false,
                          regions[i].size.x * scale, regions[i].size.y * scale,
                          0.f, COLOR(0.f, 1.f, 0.f, 0.5f), COLOR(0.f, 1.f, 0, 0.5f));
        }

        for (int i = 0; i < (int)entries.count(); ++i)
        {
            pics.draw(-1,entries[i].x * scale + posX, entries[i].y * scale + posY, 0, true,
                          8 * scale, 8 * scale,
                          0.f, COLOR(0.f, 0.f, 1.f, 0.5f), COLOR(0.f, 0.f, 1.f, 0.5f));
        }

    }

}

void GameMap::update(Actor* mainguy, PicsContainer& pics)
{
    for (unsigned i = 0; i < furniture.count(); ++i)
    {
        Furniture* fur = furniture[i];

        if (fur->removed)
        {
            continue;
        } 

        fur->colidedWithHero = false;

        if (CollisionCircleRectangle(mainguy->pos.x + mainguy->collisionBodyOffset.x,
                                 mainguy->pos.y + mainguy->collisionBodyOffset.y,
                                 mainguy->collisionBodyRadius * 2.f,
                                 fur->pos.x + fur->collisionBodyPos.x,
                                 fur->pos.y + fur->collisionBodyPos.y,
                                 fur->collisionBodySize.x,
                                 fur->collisionBodySize.y))
        {
            fur->colidedWithHero = true;
        }
    }
}

void GameMap::updateFurniturePolygons(Room* currentRoom)
{
    for (unsigned i = polygons.count() - 1; i >= polygonsBeforeFurnitureAdded; --i)
    {
        polygons[i].points.destroy();
        polygons.remove(i);
    }

    for (unsigned i = 0; i < currentRoom->getFurnitureCount(); ++i)
    {
        Furniture* fur = currentRoom->getFurniture(i);

        if (fur->removed)
        {
            continue;
        }

        SPolygon pol;

        for (unsigned j = 0; j < fur->collisionPolygon.points.count(); ++j)
        {
            Vector3D* p = &(fur->collisionPolygon.points[j]);
            pol.points.add(Vector3D(fur->pos.x + p->x, fur->pos.y + p->y, 0));
        }
        polygons.add(pol);
    }
}

void GameMap::drawFrontLayerAssets(float offsetX, float offsetY,
                                   int scale,
                                   Vector3D& characterPos,
                                   PicsContainer& pics)
{
    for (unsigned i = 0; i < assets.count(); ++i)
    {
        if (!assets[i].isInFrontLayer)
        {
            continue;
        }

        unsigned long picIndex = pics.findByName(assets[i].name);

        PicData* data = pics.getInfo(picIndex);

        float charPosY = characterPos.y + 39;

        float assetHeight = (data->sprites.count() ? data->sprites[assets[i].spriteIndex].height :
                             data->theight);



        float alpha = (charPosY - assets[i].pos.y) / assetHeight;
        if (alpha < 0.f)
        {
            alpha = 0;
        }

        pics.draw(picIndex, 
                  assets[i].pos.x * scale + offsetX + (assets[i].isFlipped ? data->htilew * 2 * scale : 0), 
                  assets[i].pos.y * scale + offsetY,
                  assets[i].spriteIndex,
                  false,
                  (assets[i].isFlipped ? -scale: scale),
                   scale,
                   0.f,
                   COLOR(1.f, 1.f, 1.f, alpha),
                   COLOR(1.f, 1.f, 1.f, alpha)

                  );
    }


}

void GameMap::drawLights(float offsetX, float offsetY, int scale, PicsContainer& pics)
{
    for (unsigned i = 0; i < assets.count(); ++i)
    {
        if (!assets[i].isLight)
        {
            continue;
        }

        unsigned long picIndex = pics.findByName(assets[i].name);

        PicData* data = pics.getInfo(picIndex);

        pics.draw(picIndex, 
                  (assets[i].pos.x + (assets[i].isFlipped ? data->htilew * 2 : 0)) * scale + offsetX, 
                  assets[i].pos.y * scale + offsetY,
                  assets[i].spriteIndex,
                  false,
                  (assets[i].isFlipped ? -1.f * scale: 1.f * scale),
                   1.f * scale
                  );

    }
}

void GameMap::drawDarknessBorder(float offsetX, float offsetY,
                                 int scale,
                                 unsigned screenWidth, unsigned screenHeight,
                                 PicsContainer& pics)
{
    if (height * 32 * scale < screenHeight)
    {
        int frame = 0;
        int frames[] = {0, 1, 1, 0, 0, 1};
        
        for (unsigned i = 0; i < width; ++i)
        {
            pics.draw(16, offsetX + i * 32 * scale + 16 * scale,
                          offsetY + (height - 1) * 32 * scale + 16 * scale,
                          frames[frame], true, scale, scale);
            pics.draw(16, offsetX + i * 32 * scale + 16 * scale,
                          offsetY + 16 * scale, frames[frame], true, scale, -scale);
        
            ++frame;

            if (frame > 5)
            {
                frame = 0;
            }
        }
    }

    if (width * 32 * scale < screenWidth)
    {
        int frame = 0;
        int frames[] = {2, 3, 3, 2, 2, 3};
        
        for (unsigned i = 0; i < height; ++i)
        {
            pics.draw(16, offsetX + 16 * scale, 
                          offsetY + i * 32 * scale + 16 * scale, 
                          frames[frame], true, scale, scale);
            pics.draw(16, offsetX + (width - 1) * 32 * scale + 16 * scale,
                          offsetY + i * 32 * scale + 16 * scale, frames[frame], true, -scale, scale);
            ++frame;

            if (frame > 5)
            {
                frame = 0;
            }

        }
    }

}


void GameMap::addItem(ItemInstance* item)
{
    items.add(item);
}

bool GameMap::getFurnitureInBBox(DArray<Furniture*>& result, Vector3D bboxPos, Vector3D bboxSize)
{
    bool res = false;

    if (!furniture.count())
    {
        return res;
    }

    for (int i = (int)furniture.count() - 1; i >= 0; --i)
    {
        Furniture* fur = furniture[i];

        if (fur->removed)
        {
            continue;
        }

        const float collisionBodyX = fur->pos.x + fur->collisionBodyPos.x;
        const float collisionBodyY = fur->pos.y + fur->collisionBodyPos.y;

        if (CollisionRectangleRectangle(bboxPos.x, bboxPos.y,
                                        bboxSize.x, bboxSize.y,
                                        collisionBodyX, collisionBodyY,
                                        fur->collisionBodySize.x,
                                        fur->collisionBodySize.y))
        {
            res = true;
            result.add(fur);
        }
    }

    return res;
}

Furniture* GameMap::getClickedFurniture(
                                        int x, int y,
                                        bool returnIfColidesWithHero)
{
    const float currentX = x;
    const float currentY = y;

    if (!furniture.count())
    {
        return nullptr;
    }

    for (int i = (int)furniture.count() - 1; i >= 0; --i)
    {
        Furniture* fur = furniture[i];

        if (fur->removed)
        {
            continue;
        }

        const float collisionBodyX = fur->pos.x + fur->collisionBodyPos.x;
        const float collisionBodyY = fur->pos.y + fur->collisionBodyPos.y;

        if (currentX > collisionBodyX
            && currentX < collisionBodyX + fur->collisionBodySize.x
                && currentY > collisionBodyY
                && currentY < collisionBodyY + fur->collisionBodySize.y)
        {
            if (returnIfColidesWithHero)
            {
                if (fur->colidedWithHero)
                {
                    return fur;
                }

            }
            else
            {
                return fur;
            }

        }

    }

    return nullptr;
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
        return containers[index];
    }

    return nullptr;
}

void GameMap::createDoorHole(float x1, float x2, float height)
{
    if (polygons.count())
    {

        int insertDoorAferIndex = -1;
        float y1 = 200;
        float y2 = 200;

        for (unsigned i = 1; i < polygons[0].points.count(); ++i)
        {
            Vector3D* point1 = &polygons[0].points[i - 1];
            Vector3D* point2 = &polygons[0].points[i];

            if (point1->x < x1 && point2->x > x2)
            {
                //y = ax + b
                float a = (point2->y - point1->y) / (point2->x - point1->x);
                float b = point1->y - point1->x * a;
 
                y1 = a * x1 + b;
                y2 = a * x2 + b;

                insertDoorAferIndex = i - 1;
                break;
            }
        }

        if (insertDoorAferIndex != -1)
        {
            printf("add door after point:%d\n", insertDoorAferIndex);
            DArray<Vector3D> copyPoints;
            
            for (unsigned i = 0; i < polygons[0].points.count(); ++i)
            {
                copyPoints.add(polygons[0].points[i]);
            }

            polygons[0].points.destroy();

            for (unsigned i = 0; i < (unsigned)insertDoorAferIndex + 1; ++i)
            {
                polygons[0].points.add(copyPoints[i]);
            }

            float smallestY = (y1 < y2) ? y1 : y2;

            polygons[0].points.add(Vector3D(x1, y1, 0));
            polygons[0].points.add(Vector3D(x1, smallestY - height, 0));
            polygons[0].points.add(Vector3D(x2, smallestY - height, 0));
            polygons[0].points.add(Vector3D(x2, y2, 0));

            for (unsigned i = insertDoorAferIndex + 1; i < copyPoints.count(); ++i)
            {
                polygons[0].points.add(copyPoints[i]);
            }

            copyPoints.destroy();
        }

    }

}

