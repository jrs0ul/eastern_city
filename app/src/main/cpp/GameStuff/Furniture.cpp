#include "Furniture.h"

void Furniture::destroy()
{
    collisionPolygon.points.destroy();
}
