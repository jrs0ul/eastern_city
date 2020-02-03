#include "Furniture.h"


void Furniture::destroy()
{
    collisionPolygon.points.destroy();
}

void Furniture::draw(float offsetX, float offsetY,
              PicsContainer& pics,
              bool debugInfo)
{
       if (removed)
        {
            return;
        }



        COLOR furnitureColor = (colidedWithHero) ? COLOR(2.f, 2.f, 2.f, 1) : COLOR(1, 1, 1, 1);
        pics.draw(pictureIndex,
                  pos.x + offsetX,
                  pos.y + offsetY,
                  spriteIndex,
                  false, 1,1, 0, furnitureColor, furnitureColor);

}


