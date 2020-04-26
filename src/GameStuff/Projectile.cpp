#include "Projectile.h"
#include "../MathTools.h"
#include "ActorContainer.h"
#include "map/GameMap.h"

Projectile::Projectile()
{
    progress = 0.f;
    alive = true;
}

void Projectile::draw(float offsetX, float offsetY,
                      int scale,
                      PicsContainer& pics,
                      bool debugInfo)
{
    pics.draw(29, pos.x * scale + offsetX, pos.y * scale + offsetY, 0, true, scale, scale);
}

void Projectile::setSource(Vector3D& v)
{
    origin = v;
    pos = origin;
}

void Projectile::setDestination(Vector3D& dest)
{
    destination = dest;
}


void Projectile::update(float deltaTime, Room* currentRoom, GameMap* map)
{
    if (!alive)
    {
        return;
    }

    const float speed = 200.f;

    Vector3D direction = destination - origin;
    direction.normalize();

    Vector3D shift(direction.x * speed * deltaTime, direction.y * speed * deltaTime, 0);

    pos = pos + shift;

    if (pos.x < -4 || pos.x > map->getWidth() * 32.f ||
            pos.y < - 4 || pos.y > map->getHeight() * 32.f)
    {
        printf("POOF! projectile is dead\n");
        alive = false;
    }

    ActorContainer* actors = map->getActorContainer();

    for (unsigned i = 0; i < actors->getActorCount(); ++i)
    {
        Actor* actor = actors->getActor(i);

        if (actor->isDead() || actor->getType() == 0 || actor->getType() == 2)
        {
            continue;
        }

        BoundingBox* bbox = actor->getCurrentTargetingBBox();

        if (CollisionCircleRectangle(pos.x, pos.y, 4,
                    actor->pos.x + bbox->pos.x,
                    actor->pos.y + bbox->pos.y,
                    bbox->size.x,
                    bbox->size.y))
        {
            alive = false;
            actor->damage(10);
        }
    }

}

