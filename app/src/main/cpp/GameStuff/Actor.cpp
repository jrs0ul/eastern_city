#include "Actor.h"
#include "../Usefull.h"

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



bool Actor::isColiding(Vector3D newPos, GameMap& map)
{
    newPos = newPos + collisionBodyOffset;

    int topX = newPos.x - collisionBodyRadius;
    int topY = newPos.y - collisionBodyRadius;
    int bottomX = newPos.x + collisionBodyRadius;
    int bottomY = newPos.y + collisionBodyRadius;

    bool colides = false;

    int fromY = topY / 32;
    int toY = bottomY / 32;
    int fromX = topX / 32;
    int toX = bottomX / 32;

    for (int i = fromY; i <= toY; ++i)
    {
        for (int j = fromX; j <= toX; ++j)
        {
            int res = map.canTraverse(j, i);

            if (res == 0)
            {
                colides = CollisionCircleRectangle(newPos.x, newPos.y, collisionBodyRadius - 1.f, j * 32, i * 32, 32, 32);

                if (colides)
                {
                    return true;
                }

            }
            else if (res == -1)
            {
                return true;
            }
        }
    }


    return false;

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
