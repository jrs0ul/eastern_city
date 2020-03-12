#ifndef PROJECTILE_CONTAINER_H
#define PROJECTILE_CONTAINER_H

#include "../TextureLoader.h"
#include "../Vectors.h"

class Projectile;
class ActorContainer;
class Room;
class GameMap;

class ProjectileContainer
{
public:
    void destroy();
    void update(float deltaTime, 
                ActorContainer& actors,
                Room* currentRoom, 
                GameMap* map);
    void addProjectile(Vector3D origin, Vector3D destination);
    void draw(float mapOffsetX, float mapOffsetY, int scale, PicsContainer& pics); 

    private:
        DArray<Projectile*> projectiles;
};

#endif //PROJECTILE_CONTAINER_H
