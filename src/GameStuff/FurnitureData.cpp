#include "FurnitureData.h"


#ifndef __ANDROID__
void FurnitureDatabase::load(const char* path)
#else
void FurnitureDatabase::load(const char* path, AAssetManager* assman)
#endif
{
    Xml datafile;
#ifndef __ANDROID__
    datafile.load(path);
#else
    datafile.load(path, assman);
#endif

    XmlNode* furnitureNode = datafile.root.getNode(L"Furniture");


    char buffer[128];

    if (furnitureNode)
    {
        for (unsigned i = 0; i < furnitureNode->childrenCount(); ++i)
        {
            XmlNode* object = furnitureNode->getNode(i);

            if (object)
            {
                FurnitureData fd;

                for (unsigned j = 0; j < object->attributeCount(); ++j)
                {
                    XmlAttribute* at = object->getAttribute(j);
                    sprintf(buffer, "%ls", at->getName());

                    if (strcmp(buffer, "name") == 0)
                    {
                        sprintf(buffer, "%ls", at->getValue());
                        strcpy(fd.name, buffer);
                    }
                    else if (strcmp(buffer, "index") == 0)
                    {
                        sprintf(buffer, "%ls", at->getValue());
                        fd.index = atoi(buffer);
                    }
                }

                XmlNode* axeDmg = object->getNode(L"AxeDamage");

                if (axeDmg)
                {
                    sprintf(buffer, "%ls", axeDmg->getValue());
                    fd.damageToAxe = atoi(buffer);
                }

                XmlNode* hp = object->getNode(L"Hp");

                if (hp)
                {
                    sprintf(buffer, "%ls", hp->getValue());
                    fd.hp = atoi(buffer);
                }


                XmlNode* spriteIndex = object->getNode(L"SpriteIndex");

                if (spriteIndex)
                {
                    sprintf(buffer, "%ls", spriteIndex->getValue());
                    fd.spriteIndex = atoi(buffer);
                }

                XmlNode* pictureIndex = object->getNode(L"PictureIndex");

                if (pictureIndex)
                {
                    sprintf(buffer, "%ls", pictureIndex->getValue());
                    fd.pictureIndex = atoi(buffer);
                }
                

                data.add(fd);
            }


        }
    }

    datafile.destroy();
}


FurnitureData* FurnitureDatabase::getFurniture(unsigned index)
{
    if (index < data.count())
    {
        return &data[index];
    }

    return nullptr;
}

void FurnitureDatabase::destroy()
{
    data.destroy();
}
