#include "GlobalItemList.h"
#include "../Xml.h"

#ifdef __ANDROID__
void GlobalItemList::load(const char* file,  AAssetManager* assman)
#else
void GlobalItemList::load(const char* file)
#endif
{
    printf("let's load %s\n", file);
    destroy();
    
    Xml itemFile;
#ifdef __ANDROID__
    itemFile.load(file, assman);
#else
    itemFile.load(file);
#endif
    printf("%s loaded; starting to parse...\n", file);
    char buffer[256];

    XmlNode* locationNode = itemFile.root.getNode(L"ItemLocations");

    if (locationNode)
    {
        for (unsigned i = 0; i < locationNode->childrenCount(); ++i)
        {
            XmlNode* groupNode = locationNode->getNode(i);

            if (groupNode)
            {
                ItemGroup group;

                if (groupNode->attributeCount())
                {
                    XmlAttribute* at = groupNode->getAttribute(0);
                    sprintf(group.mapName, "data/%ls", at->getValue());
                }

                for (unsigned j = 0; j < groupNode->childrenCount(); ++j)
                {
                    XmlNode* itemNode = groupNode->getNode(j);

                    if (itemNode)
                    {
                        ItemInstance item;
                        Vector3D pos;

                        for (unsigned k = 0; k < itemNode->attributeCount(); ++k)
                        {
                            XmlAttribute* itemAttribute = itemNode->getAttribute(k);
                            sprintf(buffer, "%ls", itemAttribute->getName());

                            if (strcmp(buffer, "x") == 0)
                            {
                                sprintf(buffer, "%ls", itemAttribute->getValue());
                                pos.x = atof(buffer);
                            }
                            else if (strcmp(buffer, "y") == 0)
                            {
                                sprintf(buffer, "%ls", itemAttribute->getValue());
                                pos.y = atof(buffer);
                            }

                        }
                        
                        pos.z = 0.f;

                        sprintf(buffer, "%ls", itemNode->getValue());

                        item.init(pos, atoi(buffer));

                        group.items.add(item);
                    }
                }

                itemList.add(group);
            }
        }
    }


    for (unsigned i = 0; i < itemList.count(); ++i)
    {
        printf(">>group name %s\n", itemList[i].mapName);
    }


    itemFile.destroy();
}

void GlobalItemList::destroy()
{
    for (unsigned i = 0; i < itemList.count(); ++i)
    {
        itemList[i].items.destroy();
    }

    itemList.destroy();
}

ItemInstance* GlobalItemList::get(const char* mapname, unsigned index)
{

    for (unsigned i = 0; i < itemList.count(); ++i)
    {
        if (strcmp(itemList[i].mapName, mapname) == 0)
        {
            if (index < itemList[i].items.count() && !itemList[i].items[index].isRemoved())
            {
                return &itemList[i].items[index];
            }
        }
    }

    return nullptr;
}

unsigned GlobalItemList::getItemCountInMap(const char* mapname)
{
    for (unsigned i = 0; i < itemList.count(); ++i)
    {
        if (strcmp(itemList[i].mapName, mapname) == 0)
        {
            return itemList[i].items.count();
        }
    }

    return 0;
}

void GlobalItemList::remove(const char* mapname, unsigned index)
{
    for (unsigned i = 0; i < itemList.count(); ++i)
    {
        if (strcmp(itemList[i].mapName, mapname) == 0)
        {
            if (index < itemList[i].items.count() && !itemList[i].items[index].isRemoved())
            {
                itemList[i].items[index].setAsRemoved();
                return;
            }
        }
    }

}

