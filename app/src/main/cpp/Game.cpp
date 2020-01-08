#include <vector>
#include <algorithm>
#include <cwchar>
#include "Game.h"
#include "GameStuff/Ghost.h"


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
   
    LoadShader(&darknessShader, "darkness");
    LoadShader(&defaultShader, "default");
    LoadShader(&colorShader, "justcolor");

    colorShader.use();



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

    //-----create fbo
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
    glGenTextures(1, &fboTexture);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //------------------

#ifdef __ANDROID__
    int glData[4];
    glGetIntegerv(GL_VIEWPORT, glData);

    if (glData)
    {
        LOGI("%d %d\n", glData[2], glData[3]);
        ScreenWidth = glData[2];
        ScreenHeight = glData[3];
    }
#endif



#ifndef __ANDROID__
    pics.load("pics/imagesToLoad.xml");
    ss.init(0);
    music.open("music/music.ogg");
    music.setVolume(sys.musicVolume);
    //music.playback();

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

    fboTextureIndex = pics.getTextureCount();
    pics.attachTexture(fboTexture, fboTextureIndex, 256, 256, 256, 256, 1);


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


    //printf("%d %d %d\n", MatrixID, MatrixID2, MatrixID3);


    FlatMatrix identity;
    MatrixIdentity(identity.m);

    FlatMatrix finalM = identity * OrthoMatrix;
    defaultShader.use();
    int MatrixID = defaultShader.getUniformID("ModelViewProjection");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, finalM.m);
    colorShader.use();
    int MatrixID2 = colorShader.getUniformID("ModelViewProjection");
    glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, finalM.m);
    darknessShader.use();
    int MatrixID3 = darknessShader.getUniformID("ModelViewProjection");
    glUniformMatrix4fv(MatrixID3, 1, GL_FALSE, finalM.m);


    

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


    glDeleteFramebuffers(1, &fbo);


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

    darknessShader.destroy();
    colorShader.destroy();
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
    map.draw(mapPosX, mapPosY, SCREEN_WIDTH, SCREEN_HEIGHT, pics, itemDB, DebugMode);
    actors.draw(mapPosX, mapPosY, pics, DebugMode);
    map.drawDarknessBorder(mapPosX, mapPosY, SCREEN_WIDTH, SCREEN_HEIGHT, pics);

    drawDarkness();

    if (stateInTheGame != GAMEPLAY)
    {
        pics.draw(-1, 0, 0, 0, false, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 
                  COLOR(0, 0, 0, (stateInTheGame == FADEIN) ? 1.f - fadeProgress : fadeProgress),
                  COLOR(0, 0, 0, (stateInTheGame == FADEIN) ? 1.f - fadeProgress : fadeProgress)
                  );
    }
   
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
    
    dude.drawInventory(pics, itemDB, selectedItem);

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
    
    int stats[] = {dude.getHealth(), dude.getSatiation(), dude.getWarmth(), dude.getWakefullness()};

    for (int i = 0; i < 4; ++i)
    {
        pics.draw(19, 270 + i * 68, 2, i);
        sprintf(buf, "%d", stats[i]);
        COLOR statColor = (stats[i] < 20) ? COLOR(1,0,0,1) : COLOR(1,1,1,1);
        WriteText(270 + i * 68 + 32, 16, pics, 0, buf, 0.8f, 0.8f, statColor, statColor);
    }

    sprintf(buf, "Temperature:%d", map.getTemperature());
    WriteText(700, 2, pics, 0, buf, 0.8f, 0.8f);
    sprintf(buf, "Day %d %dh", days, (int)(worldTime / 10));
    WriteText(700, 20, pics, 0, buf, 0.8f, 0.8f);

}

void Game::renderEditing()
{
    map.draw(mapPosX, mapPosY, SCREEN_WIDTH, SCREEN_HEIGHT, pics, itemDB, DebugMode);
}

void Game::renderTitle()
{
    glClearColor(0.5f, 0.5f, 0.6f, 1.0f);
    pics.draw(3, SCREEN_WIDTH/2.f, SCREEN_HEIGHT/2.f, 0, true);
#ifndef __ANDROID__
    WriteText(SCREEN_WIDTH/2.f - 120, SCREEN_HEIGHT/2.f + 120, pics, 0, "click anywhere to continue...");
#else
    WriteText(SCREEN_WIDTH/2.f - 110, SCREEN_HEIGHT/2.f + 120, pics, 0, "tap anywhere to continue...");
#endif
}

void Game::renderDefeat()
{
    glClearColor(0.5f, 0.5f, 0.6f, 1.0f);
    
    char buf[255];

    float totalTime = days * 240 + worldTime - 10;

    int survivedDays = totalTime / 240;

    sprintf(buf, "You have survived for %d days %d hours", survivedDays, (int)((totalTime - (survivedDays * 240)) / 10));
    WriteText(SCREEN_WIDTH/2.f - 120, 100, pics, 0, buf);

#ifndef __ANDROID__
    WriteText(SCREEN_WIDTH/2.f - 120, SCREEN_HEIGHT/2.f + 120, pics, 0, "click anywhere to continue...");
#else
    WriteText(SCREEN_WIDTH/2.f - 110, SCREEN_HEIGHT/2.f + 120, pics, 0, "tap anywhere to continue...");
#endif

}

void Game::gameLogic()
{
    centerCamera(dude.pos.x, dude.pos.y);
    calcDarknessValue();

    //---
    if (stateInTheGame != GAMEPLAY)
    {
        fadeProgress += DeltaTime * 1.8f;
        
        if (fadeProgress >= 1.f)
        {
            fadeProgress = 0.f;

            if (stateInTheGame == FADEIN)
            {
                stateInTheGame = GAMEPLAY;
            }
            else
            {
                if (!dude.isSleeping())
                {

#ifdef __ANDROID__
                    map.load(currentRoom->getMapName(), AssetManager, &itemsInWorld, currentRoom);
#else
                    map.load(currentRoom->getMapName(), &itemsInWorld, currentRoom);
#endif
                    dude.setPosition(*map.getPlayerPos(currentPlayerEntryPoint));
                    createEnemies();
                    unsigned regionIndex = 0;
                    unsigned entryIndex = 0;

                    if (dude.colidesWithRegion(map, &regionIndex, &entryIndex))
                    {
                        ignoreRegion = true;
                        printf("REGION IGNORED\n");
                    }

                    stateInTheGame = FADEIN;
                }
                else
                {
                    stateInTheGame = FADEIN;
                    printf("WELCOME TO THE WORLD OF TOMOROW!\n");
                    updateWorld(60);
                    calcDarknessValue();
                    dude.stopSleep();

                    if (darkness > 0.5f)
                    {
                        Ghost* ghost;
                        ghost = new Ghost();
                        Vector3D pos = Vector3D((map.getWidth() * 32 / 2), 240, 0);
                        ghost->init(pos);
                        actors.addActor(ghost);
                    }

                    return;
                }
            }
        }

        return;
    }
    //--
    if (dude.isSleeping() && dude.isSleepAnimationDone())
    {
        stateInTheGame = FADEOUT;
        return;
    }

   
    
    updateWorld(DeltaTime);


    if (activeContainer)
    {
        if (activeContainer->checkInput(touches, &selectedItem, itemSelected, selectedItemPos))
        {
            return;
        }
    }

    dude.checkInventoryInput(touches, &selectedItem, itemSelected, selectedItemPos);

    if (touches.move.count())
    {
        if (itemSelected)
        {
            selectedItemPos = touches.move[0];
        }
    }

    if (touches.up.count())
    {
        //----asset interactions
        Asset * ass = map.getClickedAsset(mapPosX, mapPosY, 
                                          pics, 
                                          touches.up[0].x, touches.up[1].y,
                                          true,
                                          dude.pos.x, dude.pos.y + 39);

        if (ass)
        {
            if (ass->containerIndex != -1 && (!activeContainer || !activeContainer->isActive()))
            {
                activeContainer = map.getItemContainer(ass->containerIndex);
                activeContainer->setPosition(Vector3D(touches.up[0].x, touches.up[1].y, 0));
                activeContainer->setActive(true);
                return;
            }
            else if (ass->isBed)
            {
                dude.pos.x = ass->pos.x + 113.f;
                dude.pos.y = ass->pos.y + 67.f;
                dude.isFlipedX = false;
                dude.goToSleep();
            }
        }

        //---somewhat ugly block-
        for (int i = 0; i < 10; ++i)
        {
            if (touches.up[0].x > 100 + i * 34 && touches.up[0].x < 100 + i * 34 + 32 &&
                    touches.up[0].y > 445 && touches.up[0].y < 477
               )
            {
                if (clickOnItem == i)
                {
                    printf("using item %d\n", i);
                    dude.useItem(i, itemDB);
                    itemSelected = false;
                    clickOnItem = -1;
                    return;
                }
                else
                {
                    clickOnItem = i;
                }
            }
        }
        //--

        if (itemSelected)
        {
            printf("adding selected item to map\n");
            const float newItemPosX = selectedItemPos.x - mapPosX;
            const float newItemPosY = selectedItemPos.y - mapPosY;

            printf("%f %f\n", newItemPosX, newItemPosY);

            if (!CollisionCircleRectangle(newItemPosX, newItemPosY, 8, 0, 0, map.getWidth() * 32, map.getHeight() * 32))
            {
                printf("SHIIIT\n");
                selectedItem = nullptr;
                itemSelected = false;
                return;
            }

            currentRoom->addItem(Vector3D(newItemPosX, newItemPosY, 0),
                                 selectedItem);
            map.addItem(currentRoom->getItem(currentRoom->getItemCount() - 1));
            selectedItem->setAsRemoved();
            selectedItem = nullptr;
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

        if (!map.canTraverse(dest.x, dest.y))
        {  
            Vector3D res = map.getNearestReachableSquare(dest.x, dest.y);

            if (res.x > -1 && res.y > -1)
            {
                dest.x = (int)res.x;
                dest.y = (int)res.y;
            }
        }
        
        path.destroy();
        path.findPath(src, dest, map.getCollisionData(), map.getWidth(), map.getHeight());
        dude.resetPathIndex();

        
    }


    
    
    unsigned entryIndex = 0;
    unsigned regionIndex = 0;

    bool colidesWithRegion = dude.colidesWithRegion(map, &regionIndex, &entryIndex);

    if ( colidesWithRegion && !ignoreRegion)
    {

        RoomAndEntry* rae = currentRoom->findChildRoomByRegionIndex(regionIndex);

        if (rae != nullptr)
        {
            printf("THERE'S A ROOM CONNECTED TO THIS REGION\n");
            activeContainer = nullptr;
            currentRoom = rae->room;
            currentPlayerEntryPoint = rae->entryIndex;
            path.destroy();

            stateInTheGame = FADEOUT;
        }
    }
    else if (!colidesWithRegion && ignoreRegion)
    {
        ignoreRegion = false;
    }

    if (dude.getHealth() <= 0)
    {
        gameMode = DEFEAT;
    }

}
    
void Game::updateWorld(float deltaT)
{
    worldTime += deltaT;

    if (worldTime >= 240.f)
    {
        int daysPassed = worldTime / 240;
        days += daysPassed;
        worldTime = (worldTime - daysPassed * 240);
    }
    
    dude.update(deltaT, Keys, map, darkness, itemDB, path);
    
    for (unsigned i = 0; i < actors.getActorCount(); ++i)
    {
        Actor* actr = actors.getActor(i);

        if (actr)
        {
            if (actr->getType() == 1)
            {
                Rat* rat = static_cast<Rat*>(actr);
                rat->update(deltaT, map, dude, actors);
            }
            else if (actr->getType() == 2)
            {
                Ghost* ghost = static_cast<Ghost*>(actr);
                ghost->update(deltaT, map, dude, actors);
            }
        }
    }

    map.update(&dude, pics);

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
    if (touches.up.count() && !touches.oldDown.count())
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

        glClearColor(0.f, 0.0f, 0.0f, 0.0f);
        
        ignoreRegion = false;
        selectedItem = nullptr;
        itemSelected = false;
        selectedItemPos = Vector3D(0,0,0);
        activeContainer = nullptr;
        worldTime = 100;
        days = 0;
        clickOnItem = -1;
        stateInTheGame = FADEIN;
        fadeProgress = 0.f;
    }
}

void Game::defeatLogic()
{
    
    if (touches.up.count() && !touches.oldDown.count())
    {
        gameMode = TITLE;
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

void Game::drawDarkness()
{
    if (darkness <= 0.f)
    {
        return;
    }

    pics.drawBatch(&colorShader, &defaultShader, 666);



    //draw to FBO

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    //GLenum res = glCheckFramebufferStatus(fbo);
    glViewport(0,0,256,256);
    glClear(GL_COLOR_BUFFER_BIT);
    pics.draw(11,
              dude.pos.x + mapPosX,
              dude.pos.y + mapPosY,
              0,
              true,
              1.f, 1.f,
              0.f,
              COLOR(1,1,1,0.6),
              COLOR(1,1,1,0.6f));


    if (dude.isWeaponEquiped() == 12 && dude.getAmmoInWeaponCount())
    {

        if (dude.animationSubset == 0)
        {
            pics.draw(15,
                    dude.pos.x + mapPosX + ((dude.isFlipedX) ? 89 : -89),
                    dude.pos.y + mapPosY - 20,
                    0,
                    false, (dude.isFlipedX) ? -1.6 : 1.6, 1.3);
        }
        else if (dude.animationSubset == 1)
        {

            pics.draw(11,
                    dude.pos.x + mapPosX,
                    dude.pos.y + mapPosY - 60,
                    0,
                    true,
                    1.8f, 1.5f,
                    0.f);

        }
        else
        {

            pics.draw(13,
                    dude.pos.x + mapPosX + ((dude.isFlipedX) ? 335 : -335),
                    dude.pos.y + mapPosY - 68,
                    0,
                    false, (dude.isFlipedX) ? -1.3f : 1.3f, 1.3f );
        }
    }


    pics.drawBatch(&colorShader, &defaultShader, 666);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, ScreenWidth, ScreenHeight);

    int dark  = darknessShader.getUniformID("uDarkness");
    int light = darknessShader.getUniformID("uLight");

    darknessShader.use();
    glUniform1i(dark, 0);
    glUniform1i(light, 1);


    float verts[] = {0, 0, 
        SCREEN_WIDTH, 0,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        0, 0,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        0, SCREEN_HEIGHT};

    float uvs[] = {
        0,1,//0,0,
        1,1,//1,0,
        1,0,//1,1,
        0,1,//0,0,
        1,0,//1,1,
        0,0//0,1
    };

    float colors[] = {
        0,0,0, darkness,
        0,0,0, darkness,
        0,0,0, darkness,
        0,0,0, darkness,
        0,0,0, darkness,
        0,0,0, darkness
    };


    glActiveTexture(GL_TEXTURE0 + 0); 
    glBindTexture(GL_TEXTURE_2D, pics.getname(12));

    glActiveTexture(GL_TEXTURE0 + 1); 
    glBindTexture(GL_TEXTURE_2D, pics.getname(fboTextureIndex)); 

    pics.drawVA(verts, uvs, colors, 12, 12, &darknessShader);
    glActiveTexture(GL_TEXTURE0);
    //pics.draw(fboTextureIndex, 0, 0, 0);
}

void Game::calcDarknessValue()
{
    darkness = (worldTime >= 60 && worldTime < 200) ? 1.f - sinf(((worldTime - 60) / 140.f) * M_PI) : 1.f;
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

        //if (i < currentRoom->getEnemyCount() - 1)
        {
            rat = new Rat();
            rat->init(*ratPos);
            actors.addActor(rat);
        }
        /*else
        {
            Ghost* ghost;
            ghost = new Ghost();
            ghost->init(*ratPos);
            actors.addActor(ghost);
        }*/
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

                if (weaponInfo->imageIndex != 9)
                {
                    return false;
                }

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
        case DEFEAT  : defeatLogic(); break;
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
        case DEFEAT  : renderDefeat(); break;
        case EDITING : renderEditing(); break;
    }
}

//----------------------------
void Game::DrawDebugText(){

    char buf[256];
    sprintf(buf, "FPS:%d", FPS());
    WriteText(2, 2, pics, 0, buf, 0.8f, 0.8f);
}
