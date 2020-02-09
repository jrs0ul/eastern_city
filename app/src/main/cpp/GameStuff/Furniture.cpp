#include "Furniture.h"


void Furniture::destroy()
{
    collisionPolygon.points.destroy();
}

void Furniture::draw(float offsetX, float offsetY,
                int scale,
              PicsContainer& pics,
              bool debugInfo)
{
       if (removed)
        {
            return;
        }



        COLOR furnitureColor = (colidedWithHero) ? COLOR(2.f, 2.f, 2.f, 1) : COLOR(1, 1, 1, 1);
        pics.draw(pictureIndex,
                  pos.x * scale + offsetX,
                  pos.y * scale + offsetY,
                  spriteIndex,
                  false, scale, scale, 0, furnitureColor, furnitureColor);

}


