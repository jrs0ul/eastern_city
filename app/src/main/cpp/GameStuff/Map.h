#ifndef GAME_MAP_H
#define GAME_MAP_H

class GameMap
{

    void create();
    void destroy();

    void draw(PicsContainer& pics);
    bool canTraverse(int x, int y);

private:

    bool* collision;

    unsigned width;
    unsigned height;

};


#endif //GAME_MAP_H
