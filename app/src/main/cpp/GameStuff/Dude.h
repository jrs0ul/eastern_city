#ifndef DUDE_H
#define DUDE_H

#include "../Vectors.h"
#include "../TextureLoader.h"
#include "../FindPath.h"
#include "GameMap.h"
#include "CraftingRecipes.h"
#include "ItemInstance.h"

class Dude
{
public:
    void      init(Vector3D& position);
    void      destroy(){items.destroy();}
    void      update(float deltaTime, unsigned char* Keys, GameMap& map, ItemDatabase& itemDb, Path& path);
    void      draw(float posX, float posY,
                   PicsContainer& pics, 
                   bool debugInfo = false);
    void      drawInventory(PicsContainer& pics, ItemDatabase& itemDb);
    void      useItem(unsigned index, ItemDatabase& itemDb);
    void      craftItem(Recipe* recipe, ItemDatabase& itemDb);
    bool      colidesWithRegion(GameMap& map, unsigned* regionIndex, unsigned* entryIndex);
    void      setPosition(Vector3D& position);
    void      resetPathIndex(){pathIndex = 0;}
    Vector3D* getPos(){return &pos;}
    int       getHealth(){return health;}
    int       getSatiation(){return satiation;}
private:

    bool isColiding(Vector3D newPos, GameMap& map);

private:
    DArray<ItemInstance> items;
    int                  pathIndex;
    int                  animationFrame;
    int                  animationSubset;
    bool                 isFlipedX;
    bool                 playWalkAnimation;
    bool                 walkAnimationDone;
    float                animationProgress;
    float                satiationProgress;
    float                freezingProgress;
    Vector3D             pos;

    ItemInstance*        equipedBodyItem;

    int                  maxItems;

    int                  satiation;
    float                health;
};


#endif //DUDE_H
