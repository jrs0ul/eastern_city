#include "Actor.h"
#include "../../MathTools.h"
#include <cmath>

Actor::~Actor()
{
    destroy();
}

void Actor::destroy()
{
    for (unsigned i = 0; i < animations.count(); ++i)
    {
        animations[i].frames.destroy();
    }

    animations.destroy();
}

void Actor::draw(float offsetX, float offsetY,
                 PicsContainer& pics, bool debugInfo)
{

    int frame = animations[animationSubset].frames[animationFrame];
    pics.draw(pics.findByName(spriteName), 
              pos.x + offsetX + animations[animationSubset].offsetX, 
              pos.y + offsetY + animations[animationSubset].offsetY,
              frame, 
              true, 
              (isFlipedX)? -1.f: 1.f, 
              1.f,
              0.f,
              (isDamaged) ? COLOR(1,0,0,1) : COLOR(1,1,1,1),
              (isDamaged) ? COLOR(1,0,0,1) : COLOR(1,1,1,1)
              );

    if (debugInfo)
    {
        pics.draw(-1, 
                  pos.x + offsetX + collisionBodyOffset.x,
                  pos.y + offsetY + collisionBodyOffset.y,
                  0,
                  true,
                  collisionBodyRadius * 2,
                  collisionBodyRadius * 2,
                  0.f,
                  COLOR(1,0,0,0.5f), 
                  COLOR(1,0,0,0.5f));
    }
}



bool Actor::isColiding(Vector3D newPos, Vector3D* movement, GameMap& map)
{
    newPos = newPos + collisionBodyOffset;
    bool colides = false;

    for (unsigned long i = 0; i < map.getPolygonCount(); ++i)
    {
        SPolygon* poly = map.getPolygon(i);

        if (poly->points.count() < 2)
        {
            continue;
        }


        for (unsigned long j = 1; j < poly->points.count(); ++j)
        {
            if (CollisionCircleLineSegment(poly->points[j - 1].x,
                                       poly->points[j - 1].y,
                                       poly->points[j].x,
                                       poly->points[j].y,
                                       newPos.x, newPos.y, collisionBodyRadius))
            {
                if (movement)
                {
                    Vector3D wd = Vector3D(poly->points[j].x - poly->points[j - 1].x,
                                           poly->points[j].y - poly->points[j - 1].y,
                                           0);
                    Vector3D wp1 = Vector3D(-wd.y, wd.x, 0);
                    Vector3D wp2 = Vector3D(wd.y, -wd.x, 0);
                    Vector3D normal = wp2 - wp1;
                    normal.normalize();

                    Vector3D mov = *movement;
                    
                    float angleBetweenEdgeAndMov = atan2( mov * normal , wd * mov);

                    //if circle is inside polygon angle must be > 0, outside angle > 0
                    if ((i == 0 && angleBetweenEdgeAndMov <= 0.f) 
                        || (i > 0 && angleBetweenEdgeAndMov > 0.f))
                    {
                        continue; //let's not change movement vector
                    }
                    
                    float dot = mov * normal;
                    normal.x *= dot;
                    normal.y *= dot;
                    mov = mov - normal;

                    (*movement) = mov;

                }

                colides = true;
            }

        }

    }

    return colides;


}
 
void Actor::setHealth(float newHealth)
{
    if (newHealth < health)
    {
        isDamaged = true;
        damageProgress = 0.1f;
    }

    health = newHealth;
}
