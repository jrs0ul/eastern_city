#include "ItemDatabase.h"
#include "../Xml.h"

#ifdef __ANDROID__
void ItemDatabase::load(const char* filename,  AAssetManager* assman)
#else
void ItemDatabase::load(const char* filename)
#endif

{

    Xml itemFile;
#ifdef __ANDROID__
    itemFile.load(filename, assman);
#else
    itemFile.load(filename);
#endif

    XmlNode* itemsNode = itemFile.root.getNode(L"Items");

    if (itemsNode)
    {
        for (unsigned i = 0; i < itemsNode->childrenCount(); ++i)
        {
            XmlNode* itm = itemsNode->getNode(i);

            if (itm)
            {
                ItemData data;

                char buffer[100];
                
                for (unsigned j = 0; j < itm->attributeCount(); ++j)
                {
                    XmlAttribute* at = itm->getAttribute(j);
                    sprintf(buffer, "%ls", at->getName());

                    if (strcmp(buffer, "name") == 0)
                    {
                        sprintf(data.name, "%ls", at->getValue());
                    }
                    else  if (strcmp(buffer, "isWearable") == 0)
                    {
                        sprintf(buffer, "%ls", at->getValue());
                        data.isWearable = atoi(buffer);
                    }
                    else  if (strcmp(buffer, "isConsumable") == 0)
                    {
                        sprintf(buffer, "%ls", at->getValue());
                        data.isConsumable = atoi(buffer);
                    }

                    else  if (strcmp(buffer, "coldDecrease") == 0)
                    {
                        sprintf(buffer, "%ls", at->getValue());
                        data.coldDecrease = atoi(buffer);
                    }
                    else  if (strcmp(buffer, "spoilageSpeed") == 0)
                    {
                        sprintf(buffer, "%ls", at->getValue());
                        data.spoilageSpeed = atof(buffer);
                    }
                    else  if (strcmp(buffer, "imageIndex") == 0)
                    {
                        sprintf(buffer, "%ls", at->getValue());
                        data.imageIndex = atoi(buffer);
                    }
                    else  if (strcmp(buffer, "hungerDecrease") == 0)
                    {
                        sprintf(buffer, "%ls", at->getValue());
                        data.hungerDecrease = atoi(buffer);
                    }
                    else  if (strcmp(buffer, "hpUp") == 0)
                    {
                        sprintf(buffer, "%ls", at->getValue());
                        data.hpUp = atoi(buffer);
                    }
                    
                }
                
                items.add(data);
            }
        }
    }

    itemFile.destroy();

}

void ItemDatabase::destroy()
{
    items.destroy();
}

ItemData* ItemDatabase::get(unsigned index)
{
    if (index < items.count())
    {
        return &items[index];
    }

    return nullptr;
}
