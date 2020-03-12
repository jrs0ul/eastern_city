#include "Actor.h"
#include "../../MathTools.h"
#include <cmath>
#include <cassert>

Actor::Actor()
: roomIsIn(nullptr)
, mapIsIn(nullptr)
{
}

Actor::~Actor()
{
    destroy();
}

void Actor::init(Room* currentRoom, GameMap* currentMap)
{
    dead = false;
    isDamaged = false;
    flippedX = false;

    roomIsIn = currentRoom;
    mapIsIn = currentMap;
}

void Actor::destroy()
{
    for (unsigned i = 0; i < animations.count(); ++i)
    {
        animations[i].frames.destroy();
    }

    loot.destroy();
    animations.destroy();
}

    
void Actor::dropLoot(Room* room, GameMap* map)
{
    assert(room && map);

    printf("currentRoom %d\n", room);
    printf("Dropping LOOT:\n");
    for (unsigned i = 0; i < loot.count(); ++i)
    {
        printf("item %d\n", loot[i]);
        room->addItem(Vector3D(pos.x + rand() % 10, pos.y + rand() % 10, 0), loot[i]);
        map->addItem(room->getItem(room->getItemCount() - 1));
    }
}

void Actor::draw(float offsetX, float offsetY,
                 int scale,
                 PicsContainer& pics, bool debugInfo)
{
    if (animationSubset >= (int)animations.count())
    {
        printf("ERROR: animation subset %d is out of bounds\n", animationSubset);
    }

    //printf("frame count in subset %d : %lu, current frame %d\n", animationSubset, animations[animationSubset].frames.count(), animationFrame);
    int frame = animations[animationSubset].frames[animationFrame];
    
    pics.draw(pictureIndex, 
              pos.x * scale + offsetX + animations[animationSubset].offsetX * scale, 
              pos.y * scale + offsetY + animations[animationSubset].offsetY * scale,
              frame, 
              true, 
              (flippedX)? -scale: scale, 
              scale,
              0.f,
              (isDamaged) ? COLOR(1,0,0,1) : COLOR(1,1,1,1),
              (isDamaged) ? COLOR(1,0,0,1) : COLOR(1,1,1,1)
              );

    if (debugInfo)
    {
        pics.draw(-1, 
                  pos.x * scale + offsetX + collisionBodyOffset.x * scale,
                  pos.y * scale + offsetY + collisionBodyOffset.y * scale,
                  0,
                  true,
                  collisionBodyRadius * 2 * scale,
                  collisionBodyRadius * 2 * scale,
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

    if (health <= 0 && !dead)
    {
        kill();
        dropLoot(roomIsIn, mapIsIn);
    }

}

void Actor::damage(float dmg)
{
    setHealth(health - dmg);
}
    
void Actor::updateDamage(float deltaTime)
{
    if (isDamaged)
    {
        damageProgress -= deltaTime;

        if (damageProgress <= 0.f)
        {
            isDamaged = false;
        }
    }

}
