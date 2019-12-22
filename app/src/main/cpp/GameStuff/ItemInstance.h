#ifndef ITEM_INSTANCE_H
#define ITEM_INSTANCE_H

#include "../TextureLoader.h"
#include "../Vectors.h"

class ItemInstance
{
public:
    void      draw(PicsContainer& pics);
    void      init(Vector3D position, int index);
    Vector3D* getPosition(){return &pos;}
    int       getIndex(){return itemIndex;}
    bool      isRemoved(){return removed;}
    float     getQuality(){return quality;}
    void      setQuality(float q){quality = q;}
    void      setAsRemoved(){removed = true;}

private:

    int         itemIndex;
    Vector3D    pos;
    float       quality;
    bool        removed;

};


#endif //ITEM_INSTANCE_H
