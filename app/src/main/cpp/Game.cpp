#include <vector>
#include <algorithm>
#include <cwchar>
#include "Game.h"


const int boardX = 16;
const int boardY = 445;
const int tileSize = 64;


//--------------------------------------
void Game::loadConfig(){
#ifndef __ANDROID__
    char buf[1024];
    sprintf(buf, "%s/settings.cfg", DocumentPath);
    sys.load(buf);
    ScreenWidth = sys.ScreenWidth;
    ScreenHeight = sys.ScreenHeight;
    windowed = sys.useWindowed;

    sys.write(buf);
#endif

}
//---------------------------------------
void Game::LoadShader(ShaderProgram* shader, const char* name)
{
    shader->create();

    char error[1024];
    char buf[512];

    Shader vert;
    Shader frag;

    printf("Loading vertex shader...\n");
    sprintf(buf, "shaders/%s.vert", name);
#ifdef __ANDROID__
    vert.load(GL_VERTEX_SHADER, buf, AssetManager);
#else
    vert.load(GL_VERTEX_SHADER, buf);
#endif

    printf("Loading fragment shader...\n");
    sprintf(buf, "shaders/%s.frag", name);
#ifdef __ANDROID__
    frag.load(GL_FRAGMENT_SHADER, buf, AssetManager);
#else
    frag.load(GL_FRAGMENT_SHADER, buf);
#endif

    shader->attach(vert);
    shader->attach(frag);
    shader->link();

    shader->getLog(error, 1024);
    if (strlen(error)) {
#ifdef __ANDROID__
        LOGI("--%s--", buf);
        LOGI("%s", error);
    }
    LOGI("---------------");
#else
        printf("--%s--\n", buf);
        puts(error);
    }
    puts("-----------");
#endif
}



void Game::restartGame()
{
}



//----------------------------------------
void Game::init(){

    srand(time(0));

#ifndef __ANDROID__
    LoadExtensions();
#endif
    printf("Creating shaders...\n");
   
    LoadShader(&defaultShader, "default");
    LoadShader(&colorShader, "justcolor");
    LoadShader(&modelShader, "model");

    colorShader.use();

    glClearColor(0.5f, 0.5f, 0.6f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glDepthFunc(GL_LEQUAL);

    glEnable (GL_BLEND);

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CW); 


    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    //glAlphaFunc(GL_GREATER, 0.5);
    //glEnable(GL_ALPHA_TEST);
    

    MatrixOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, -400, 400, OrthoMatrix);



#ifndef __ANDROID__
    pics.load("pics/imagesToLoad.xml");
    ss.init(0);
    music.open("music/music.ogg");
    music.setVolume(sys.musicVolume);
    music.playback();

#else
    pics.load("pics/imagesToLoad.xml", AssetManager);
    if (!ss.init()) {
        LOGI("FAIL!\n");
        ss.exit();
    }
    //ss.loadFiles("sfx/", "list.txt", AssetManager);
    ss.playMusic("music/music.ogg", AssetManager);



    LOGI("Inited\n");
#endif


    useAccel = false;

    srand((unsigned int)time(0));





    printf("game mode:%d\n", gameMode);
    if (gameMode == EDITING)
    {
        printf("about to load %s\n", fileName);
#ifdef __ANDROID__
        map.load(fileName, AssetManager);
#else
        map.load(fileName);
#endif
    }
    else
    {
#ifdef __ANDROID__
        itemDB.load("data/items.xml", AssetManager);
        recipes.load("data/recipes.xml", AssetManager);
#else
        itemDB.load("data/items.xml");
        recipes.load("data/recipes.xml");
#endif
    }

}

//-------------------
void Game::render(){
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int MatrixID = defaultShader.getUniformID("ModelViewProjection");
    FlatMatrix identity;
    MatrixIdentity(identity.m);

    int MatrixID2 = colorShader.getUniformID("ModelViewProjection");
    modelMatrixId = modelShader.getUniformID("ModelViewProjection");



    FlatMatrix finalM = identity * OrthoMatrix;
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, finalM.m);
    glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, finalM.m);


    

    Render2D();
    pics.drawBatch(&colorShader, &defaultShader, 666);

    
}

//---------------------
void Game::logic(){
#ifndef __ANDROID__
    if (music.playing())
        music.update();
#endif

    GameLoop();
    
    touches.oldDown.destroy();
    for (unsigned long i = 0; i < touches.down.count(); i++ ){
        Vector3D v = touches.down[i];
        touches.oldDown.add(v);
    }
    touches.up.destroy();
    touches.down.destroy();
    touches.move.destroy();
}
//-------------------------
void Game::destroy(){


#ifndef __ANDROID__
    music.release();
    ss.freeData();
    ss.exit();
#else
    ss.stopMusic();
    ss.exit();
#endif
    pics.destroy();

    actors.destroy();
    map.destroy();
    path.destroy();

    recipes.destroy();
    itemDB.destroy();
    dude.destroy();
    itemsInWorld.destroy();
    mapGraph.destroy();

    defaultShader.destroy();
}

//==================================================
int Game::FPS (void)
{
    static int ctime = 0, FPS = 0, frames = 0, frames0 = 0;
    if ((int)TimeTicks >= ctime) {
        FPS = frames - frames0;
        ctime = (int)TimeTicks + 1000;
        frames0 = frames;
    }
    frames = frames+1;
    return FPS;
}


//---------------------------
void Game::onBack()
{

}

void Game::renderGame()
{
    map.draw(mapPosX, mapPosY, pics, itemDB, DebugMode);
   
    actors.draw(mapPosX, mapPosY, pics, DebugMode);

    //dude.draw(mapPosX, mapPosY, pics, DebugMode);

    if (DebugMode)
    {
        for (unsigned i = 0; i < path.parent.count(); i++)
        {
            pics.draw(-1, path.parent[i].x * 32 + mapPosX, 
                    path.parent[i].y * 32 + mapPosY,
                    0, false, 32, 32, 0,
                    COLOR(0,0,1,0.5),
                    COLOR(0,0,1,0.5));

        }
    }
    
    dude.drawInventory(pics, itemDB);

    drawActiveContainer();

    if (itemSelected)
    {
        pics.draw(4, selectedItemPos.x, selectedItemPos.y, selectedItem->getIndex(), true);
    }


    char buf[256];
    WriteText(10, 10, pics, 0, "Craft:", 0.8f, 0.8f);

    for (unsigned i = 0; i < recipes.getRecipeCount(); ++i)
    {

        Recipe* recipe = recipes.getRecipe(i);

        pics.draw(2, 10, 32 + i * 34, 0, false, 0.25f, 0.5);
        pics.draw(2, 10 + 16, 32 + i * 34, 1, false, 0.25f, 0.5);

        pics.draw(4, 10, 32 + i * 34, recipe->indexOfItemToMake);
        
    }

    if (DebugMode)
    {
        DrawDebugText();
    }
    
    sprintf(buf, "Satiation:%d", dude.getSatiation());
    WriteText(500, 2, pics, 0, buf, 0.8f, 0.8f);
    sprintf(buf, "Health:%d", dude.getHealth());
    WriteText(500, 20, pics, 0, buf, 0.8f, 0.8f);

}

void Game::renderEditing()
{
    map.draw(mapPosX, mapPosY, pics, itemDB, DebugMode);
}

void Game::renderTitle()
{
    pics.draw(3, SCREEN_WIDTH/2.f, SCREEN_HEIGHT/2.f, 0, true);
}

void Game::gameLogic()
{
    centerCamera(dude.pos.x, dude.pos.y);

    if (activeContainer)
    {
        if (activeContainer->checkInput(touches, &selectedItem, itemSelected, selectedItemPos))
        {
            return;
        }
    }
  
    if (touches.down.count())
    {
        for (int i = 0; i < 10; ++i)
        {
            if (touches.down[0].x > 100 + i * 34 && touches.down[0].x < 100 + i * 34 + 32 &&
                    touches.down[0].y > 445 && touches.down[0].y < 477 && !itemSelected
               )
            {
                if (i < (int)dude.getItemCount() && !dude.getItem(i)->isRemoved())
                {
                    printf("Item selected\n");
                    selectedItem = dude.getItem(i);
                    itemSelected = true;
                    selectedItemPos = Vector3D(touches.down[0].x, touches.down[0].y, 0);
                }
            }
        }

    }

    if (touches.move.count())
    {
        if (itemSelected)
        {
            selectedItemPos = touches.move[0];
        }
    }

    if (touches.up.count())
    {
        Asset * ass = map.getClickedAsset(mapPosX, mapPosY, 
                                          pics, 
                                          touches.up[0].x, touches.up[1].y);

        if (ass)
        {
            printf("clicked on asset %s sprite:%d container:%d\n", ass->name, ass->spriteIndex, ass->containerIndex);

            if (ass->containerIndex != -1)
            {
                printf("Activating item container\n");
                activeContainer = map.getItemContainer(ass->containerIndex);
                activeContainer->setPosition(Vector3D(touches.up[0].x, touches.up[1].y, 0));
                activeContainer->setActive(true);
                return;
            }
        }

        for (int i = 0; i < 10; ++i)
        {
            if (touches.up[0].x > 100 + i * 34 && touches.up[0].x < 100 + i * 34 + 32 &&
                    touches.up[0].y > 445 && touches.up[0].y < 477
               )
            {
                printf("using item %d\n", i);
                dude.useItem(i, itemDB);
                itemSelected = false;
                return;
            }
        }

        if (itemSelected)
        {
            printf("adding selected item to map\n");
            currentRoom->addItem(Vector3D(selectedItemPos.x - mapPosX, selectedItemPos.y - mapPosY, 0),
                                 selectedItem->getIndex());
            map.addItem(currentRoom->getItem(currentRoom->getItemCount() - 1));
            selectedItem->setAsRemoved();
            itemSelected = false;
            return;
        }
        
        if (handleCrafting(touches.up[0].x, touches.up[0].y))
        {
            return;
        }

        if (handleShooting(touches.up[0].x - mapPosX, touches.up[0].y - mapPosY))
        {
            return;
        }

        _Point src;
        Vector3D* dudePos = dude.getPos();
        src.x = dudePos->x / 32; 
        src.y = (dudePos->y + 39) / 32;
        _Point dest;
        dest.x = (touches.up[0].x - mapPosX) / 32;
        dest.y = (touches.up[0].y - mapPosY) / 32;
        path.destroy();
        path.findPath(src, dest, map.getCollisionData(), map.getWidth(), map.getHeight());
        dude.resetPathIndex();

        
    }


    dude.update(DeltaTime, Keys, map, itemDB, path);
    
    for (unsigned i = 0; i < actors.getActorCount(); ++i)
    {
        Actor* actr = actors.getActor(i);

        if (actr)
        {
            if (actr->getType() == 1)
            {
                Rat* rat = static_cast<Rat*>(actr);
                rat->update(DeltaTime, map, dude, actors);
            }
        }
    }

    unsigned entryIndex = 0;
    unsigned regionIndex = 0;

    bool colidesWithRegion = dude.colidesWithRegion(map, &regionIndex, &entryIndex);

    if ( colidesWithRegion && !ignoreRegion)
    {

        RoomAndEntry* rae = currentRoom->findChildRoomByRegionIndex(regionIndex);

        if (rae != nullptr)
        {
            activeContainer = nullptr;
            currentRoom = rae->room;
            printf("THERE'S A ROOM CONNECTED TO THIS REGION\n");
#ifdef __ANDROID__
            map.load(currentRoom->getMapName(), AssetManager, &itemsInWorld, currentRoom);
#else
            map.load(currentRoom->getMapName(), &itemsInWorld, currentRoom);
#endif
            path.destroy();
            dude.setPosition(*map.getPlayerPos(rae->entryIndex));
            createEnemies();

            if (dude.colidesWithRegion(map, &regionIndex, &entryIndex))
            {
                ignoreRegion = true;
            }
        }
    }
    else if (!colidesWithRegion && ignoreRegion)
    {
        ignoreRegion = false;
    }

    if (dude.getHealth() <= 0)
    {
        gameMode = TITLE;
    }

}

void Game::editingLogic()
{
    if (touches.down.count())
    {
        int x = (touches.down[0].x - mapPosX) / 32;
        int y = (touches.down[0].y - mapPosY) / 32;

        //printf("%d %d\n", x, y);

        map.setCollision(x, y, Keys[4]);

    }
    else if (touches.move.count())
    {
        int x = (touches.move[0].x - mapPosX) / 32;
        int y = (touches.move[0].y - mapPosY) / 32;

        //printf("%d %d\n", x, y);
        map.setCollision(x, y, Keys[4]);
    }

    if (Keys[0])
    {
        mapPosY += 4;
    }

    if (Keys[1])
    {
        mapPosY -= 4;
    }

    if (Keys[2])
    {
        mapPosX += 4;
    }

    if (Keys[3])
    {
        mapPosX -= 4;
    }

    if (OldKeys[5] && !Keys[5])
    {
        printf("SAVING %s...\n", fileName);
        map.save(fileName);
    }

}

void Game::titleLogic()
{
    if (touches.up.count())
    {
        //mapGraph.init();
        mapGraph.destroy();
        mapGraph.init();
        currentRoom = mapGraph.root;
#ifdef __ANDROID__
        itemsInWorld.load("data/itemLocations.xml", AssetManager);
#else
        itemsInWorld.load("data/itemLocations.xml");
#endif
        gameMode = GAME;
        dude.destroy();
#ifdef __ANDROID__
        map.load(currentRoom->getMapName(), AssetManager, &itemsInWorld, currentRoom);
#else
        map.load(currentRoom->getMapName(), &itemsInWorld, currentRoom);
#endif
        dude.init(*map.getPlayerPos(0));

        createEnemies();

        
        ignoreRegion = false;
        selectedItem = nullptr;
        itemSelected = false;
        selectedItemPos = Vector3D(0,0,0);
        activeContainer = nullptr;
    }

}


void Game::drawActiveContainer()
{
    if (!activeContainer)
    {
        return;
    }

    if (!activeContainer->isActive())
    {
        return;
    }

    activeContainer->draw(pics, itemDB, selectedItem);

}


void Game::centerCamera(float x, float y)
{
    const float mapWidth = map.getWidth() * 32.f;
    const float mapHeight = map.getHeight() * 32.f;

    if (mapWidth > SCREEN_WIDTH)
    {
        mapPosX = SCREEN_WIDTH / 2.f - dude.pos.x;

        if (mapPosX > 0)
        {
            mapPosX = 0;
        }

        if (mapPosX + mapWidth < SCREEN_WIDTH)
        {
            mapPosX =  SCREEN_WIDTH - mapWidth;
        }
    }
    else
    {
        mapPosX = SCREEN_WIDTH / 2.f - mapWidth / 2.f;
    }
//----
    if (mapHeight > SCREEN_HEIGHT)
    {
        mapPosY = SCREEN_HEIGHT / 2.f - dude.pos.y;

        if (mapPosY > 0)
        {
            mapPosY = 0;
        }

        if (mapPosY + mapHeight < SCREEN_HEIGHT)
        {
            mapPosY = SCREEN_HEIGHT - mapHeight;
        }
    }
    else
    {
        mapPosY = SCREEN_HEIGHT / 2.f  - mapHeight / 2.f;
    }


}

void Game::createEnemies()
{
    actors.destroy();

    for (unsigned i = 0; i < currentRoom->getEnemyCount(); ++i)
    {
        Rat* rat;
        Vector3D* ratPos = currentRoom->getEnemyPosition(i);
        rat = new Rat();
        rat->init(*ratPos);
        actors.addActor(rat);
    }

    actors.addActor(&dude);

}

bool Game::handleShooting(float x, float y)
{
    for (unsigned i = 0; i < actors.getActorCount(); ++i)
    {
        Actor* actor = actors.getActor(i);

        if (actor->getType() != 1)
        {
            continue;
        }


        if (CollisionCircleCircle(x, y, 1.f, actor->pos.x, actor->pos.y, 30))
        {
            int weaponEquiped = dude.isWeaponEquiped();

            if (weaponEquiped != -1)
            {
                ItemData* weaponInfo = itemDB.get(weaponEquiped); 

                int ammoSlot = dude.hasItem(weaponInfo->ammoItemIndex);

                if (ammoSlot != -1)
                { 
                    if (actor->isDead)
                    {
                        return false;
                    }
            
                    dude.removeItem(ammoSlot);
                    actor->kill();
                    currentRoom->addItem(Vector3D(actor->pos.x, actor->pos.y, 0), 1);
                    map.addItem(currentRoom->getItem(currentRoom->getItemCount() - 1));
                }
            }

            return true;
        }
    }

    return false;

}

bool Game::handleCrafting(float x, float y)
{
    for (unsigned i = 0; i < recipes.getRecipeCount(); ++i)
    {
        if (x > 10 && x < 42 &&
                y > 32 + i * 34 && y < 64 + i * 34)
        {
            printf("let's craft some shit\n");

            Recipe* recipe = recipes.getRecipe(i);
            dude.craftItem(recipe, itemDB);
            return true;
        }
    }

    return false;

}

void Game::GameLoop(){

    switch (gameMode)
    {
        case GAME    : gameLogic(); break;
        case TITLE   : titleLogic(); break;
        case EDITING : editingLogic(); break;
    }
}

//---------------------------

void Game::Render2D()
{
    switch(gameMode)
    {
        case GAME    : renderGame(); break;
        case TITLE   : renderTitle(); break;
        case EDITING : renderEditing(); break;
    }
}

//----------------------------
void Game::DrawDebugText(){

    char buf[256];
    sprintf(buf, "FPS:%d", FPS());
    WriteText(2, 2, pics, 0, buf, 0.8f, 0.8f);
    
    
}
