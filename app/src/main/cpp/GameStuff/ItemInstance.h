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
    int       getAmmoLoaded(){return ammoLoaded;}
    bool      isRemoved(){return removed;}
    float     getQuality(){return quality;}
    void      setAmmoLoaded(int count){ammoLoaded = count;}
    void      setQuality(float q){quality = q;}
    void      setAsRemoved(){removed = true;}

private:

    int         itemIndex;
    Vector3D    pos;
    int         ammoLoaded;
    float       quality;
    bool        removed;

};


#endif //ITEM_INSTANCE_H
