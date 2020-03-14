#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Drawable.h"

class ActorContainer;
class Room;
class GameMap;

class Projectile : public Drawable
{
public:
    Projectile();
    void draw(float offsetX, float offsetY,
                      int scale,
                      PicsContainer& pics,
                      bool debugInfo = false) override;
    void setSource(Vector3D& v);
    void setDestination(Vector3D& dest);
    void update(float deltaTime, 
                Room* currentRoom,
                GameMap* map);
    bool isAlive(){return alive;}
private:
    Vector3D destination;
    Vector3D origin;
    float progress;
    bool  alive;
};

#endif // PROJECTILE_H
