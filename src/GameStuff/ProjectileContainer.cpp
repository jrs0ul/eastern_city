#include "ProjectileContainer.h"
#include "Projectile.h"

void ProjectileContainer::destroy()
{
    for (unsigned i = 0; i < projectiles.count(); ++i)
    {
        delete projectiles[i];
    }

    projectiles.destroy();
}

void ProjectileContainer::update(float deltaTime,
                                 Room* currentRoom, 
                                 GameMap* map)
{
    for (unsigned i = 0; i < projectiles.count(); ++i)
    {
        projectiles[i]->update(deltaTime, currentRoom, map);
    }

    for (int i = (int)projectiles.count() - 1; i >= 0; --i)
    {
        if (!projectiles[i]->isAlive())
        {
            projectiles.remove(i);
        }
    }
}

void ProjectileContainer::addProjectile(Vector3D origin, Vector3D destination)
{
    Projectile* proj = new Projectile();
    proj->setSource(origin);
    proj->setDestination(destination);

    projectiles.add(proj);
}


void ProjectileContainer::draw(float mapOffsetX, float mapOffsetY, int scale, PicsContainer& pics)
{
    for (unsigned i = 0; i < projectiles.count(); ++i)
    {
        projectiles[i]->draw(mapOffsetX, mapOffsetY, scale, pics);
    }
}

