#include <vector>
#include <cwchar>
#include <cmath>
#include "Game.h"
#include "GameStuff/actors/Ghost.h"
#include "GameStuff/actors/Bear.h"

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

#ifdef __ANDROID__
    int glData[4];
    glGetIntegerv(GL_VIEWPORT, glData);

    if (glData)
    {
        LOGI("%dx%d\n", glData[2], glData[3]);
        ScreenWidth = glData[2];
        ScreenHeight = glData[3];
    }
#endif

    MatrixOrtho(0.0, ScreenWidth, ScreenHeight, 0.0, -400, 400, OrthoMatrix);

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


    pixelArtScale = ScreenWidth / 640;



#ifndef __ANDROID__
    pics.load("pics/imagesToLoad.xml");
    SoundSystem::getInstance()->init(0);
    SoundSystem::getInstance()->loadFiles("sfx/", "list.txt");
    music.open("music/music.ogg");
    music.setVolume(sys.musicVolume);
    //music.playback();

#else
    pics.load("pics/imagesToLoad.xml", AssetManager);
    /*if (!ss.init()) {
        LOGI("FAIL!\n");
        ss.exit();
    }
    ss.loadFiles("sfx/", "list.txt", AssetManager);
    ss.playMusic("music/music.ogg", AssetManager);*/



    LOGI("Inited\n");
#endif

    fboTextureIndex = pics.getTextureCount();
    pics.attachTexture(fboTexture, fboTextureIndex, 256, 256, 256, 256, 1);


    useAccel = false;

    srand((unsigned int)time(0));

    printf("game mode:%d\n", gameMode);

#ifdef __ANDROID__
    itemDB.load("data/items.xml", AssetManager);
    recipes.load("data/recipes.xml", AssetManager);
    furnitureDB.load("data/furniture.xml", AssetManager);
#else
    itemDB.load("data/items.xml");
    recipes.load("data/recipes.xml");
    furnitureDB.load("data/furniture.xml");
#endif

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

    Statistics::getInstance()->send(days * dayLength + worldTime - ((dayLength / 24) * 10), time(0) - sessionStarted, true, statsPostRequest);
    sendStats = true;

    glDeleteFramebuffers(1, &fbo);


#ifndef __ANDROID__
    music.release();
    SoundSystem* ss = SoundSystem::getInstance();
    ss->freeData();
    ss->exit();
#else
    //ss.stopMusic();
    //ss.exit();
#endif
    pics.destroy();

    map.destroy();
    path.destroy();

    recipes.destroy();
    furnitureDB.destroy();
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
    const int scale = pixelArtScale;

    map.draw(mapPosX, mapPosY, scale, ScreenWidth, ScreenHeight, pics, itemDB, DebugMode);
    projectiles.draw(mapPosX, mapPosY, scale, pics);
    map.drawFrontLayerAssets(mapPosX, mapPosY, scale, *dude.getPos(), pics);
    map.drawDarknessBorder(mapPosX, mapPosY, scale, ScreenWidth, ScreenHeight, pics);

    if (DebugMode)
    {
        pics.drawBatch(&colorShader, &defaultShader, 666);

        colorShader.use();

        glDisable(GL_TEXTURE_2D);
        glLineWidth(4.f);

        for (unsigned long i = 0; i < map.getPolygonCount(); ++i)
        {
            SPolygon* poly = map.getPolygon(i);
            drawPolygon(poly, scale, colorShader);
        }

        SPolygon pp;

        for (unsigned long i = 0; i < path.getPathLength(); ++i)
        {
            Vector3D point = *(path.getPathStep(i));
            pp.points.add(point);
        }

        drawPolygon(&pp, scale, colorShader, GL_LINE_STRIP, COLOR(1,0,0,1));
        pp.points.destroy();

        #ifndef __ANDROID__
        glPointSize(10.f);
        #endif

        for (unsigned i = 0; i < path.getNodeCount(); ++i)
        {
            Vector3D point = *(path.getNodePos(i));
            pp.points.add(point);
        }

        drawPolygon(&pp, scale, colorShader, GL_POINTS, COLOR(1,0,1,1));
        pp.points.destroy();

        for (unsigned i = 0; i < path.getDebugPointsCount(); ++i)
        {
            Vector3D point = *(path.getDebugPoint(i));
            pp.points.add(point);
        }

        drawPolygon(&pp, scale, colorShader, GL_POINTS, COLOR(1,1,0,1));
        pp.points.destroy();


        
        glEnable(GL_TEXTURE_2D);
    }

    drawDarkness(scale);

    if (stateInTheGame != GAMEPLAY)
    {
        pics.draw(-1, 0, 0, 0, false, ScreenWidth, ScreenHeight, 0, 
                  COLOR(0, 0, 0, (stateInTheGame == FADEIN) ? 1.f - fadeProgress : fadeProgress),
                  COLOR(0, 0, 0, (stateInTheGame == FADEIN) ? 1.f - fadeProgress : fadeProgress)
                  );
    }

    mapGraph.drawYardMap(ScreenWidth - (yardMapWidth + 2) * 16,
                         64,
                         pics, currentRoom);
    
    dude.drawInventory(pics, itemDB, selectedItem, pixelArtScale);

    drawActiveContainer();

    if (itemSelected)
    {
        pics.draw(4, 
                  selectedItemPos.x, 
                  selectedItemPos.y, 
                  itemDB.get(selectedItem->getIndex())->imageIndex,
                  true, pixelArtScale, pixelArtScale);
    }


    char buf[256];
  
    drawRecipes(pixelArtScale);

    if (DebugMode)
    {
        DrawDebugText();
    }
    
    int stats[] = {dude.getHealth(), dude.getSatiation(), dude.getWarmth(), dude.getWakefullness()};

    const float hudStartX = (ScreenWidth / 2) - ((2 * 68 + 16) * scale);

    for (int i = 0; i < 4; ++i)
    {
        pics.draw(19, hudStartX + (i * 68) * scale, 2 * scale, i, false, scale, scale);
        sprintf(buf, "%d", stats[i]);
        COLOR statColor = (stats[i] < 20) ? COLOR(1,0,0,1) : COLOR(1,1,1,1);
        WriteText(hudStartX + (i * 68 + 32) * scale, 16 * scale, pics, 0, buf, 0.8f * scale, 0.8f * scale, statColor, statColor);
    }

    sprintf(buf, "Temperature:%d", map.getTemperature());
    WriteText(ScreenWidth - (scale * 150), 2 * scale, pics, 0, buf, 0.8f * scale, 0.8f * scale);
    sprintf(buf, "Day %d %dh", days, (int)(worldTime / (dayLength / 24)));
    WriteText(ScreenWidth - (150 * scale), 20 * scale, pics, 0, buf, 0.8f * scale, 0.8f * scale);

    int weaponEquiped = dude.isWeaponEquiped();
    ItemData* weaponInfo = itemDB.get(weaponEquiped);
    
    bool isSlingShot = (weaponInfo && weaponInfo->imageIndex == 9);

#ifndef __ANDROID__
    pics.draw(31, MouseX, MouseY, (isSlingShot) ? 1 : 0, isSlingShot, scale, scale); 
#endif
}

void Game::renderEditing()
{
    map.draw(mapPosX, mapPosY, 2, ScreenWidth, ScreenHeight, pics, itemDB, DebugMode);
}

void Game::renderTitle()
{
    glClearColor(0.5f, 0.5f, 0.6f, 1.0f);
    pics.draw(3, ScreenWidth/2.f, ScreenHeight/2.f, 0, true, pixelArtScale, pixelArtScale);
#ifndef __ANDROID__
    WriteText(ScreenWidth/2.f - 120 * pixelArtScale,
              ScreenHeight/2.f + 120 * pixelArtScale, pics, 0, "click anywhere to continue...", pixelArtScale, pixelArtScale);
    pics.draw(31, MouseX, MouseY, 0, false, pixelArtScale, pixelArtScale); 
#else
    WriteText(ScreenWidth/2.f - 110 * pixelArtScale,
              ScreenHeight/2.f + 120 * pixelArtScale,
              pics, 0, "tap anywhere to continue...", pixelArtScale, pixelArtScale);
#endif
}

void Game::renderDefeat()
{
    glClearColor(0.5f, 0.5f, 0.6f, 1.0f);
    
    char buf[255];

    float totalTime = days * dayLength + worldTime - ((dayLength / 24) * 10);

    int survivedDays = totalTime / dayLength;

    sprintf(buf, "You have survived for %d days %d hours", survivedDays, (int)((totalTime - (survivedDays * dayLength)) / (dayLength / 24)));
    WriteText(ScreenWidth/2.f - 120 * pixelArtScale, 100 * pixelArtScale, pics, 0, buf, pixelArtScale, pixelArtScale);

#ifndef __ANDROID__
    WriteText(ScreenWidth/2.f - 120 * pixelArtScale, ScreenHeight/2.f + 120 * pixelArtScale, pics, 0, "click anywhere to continue...", pixelArtScale, pixelArtScale);
    pics.draw(31, MouseX, MouseY, 0, false, pixelArtScale, pixelArtScale); 
#else
    WriteText(ScreenWidth/2.f - 110 * pixelArtScale, ScreenHeight/2.f + 120 * pixelArtScale, pics, 0, "tap anywhere to continue...", pixelArtScale, pixelArtScale);
#endif

}

void Game::gameLogic()
{
    int scale = pixelArtScale;

    centerCamera(dude.pos.x, dude.pos.y, scale);
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
                    map.load(currentRoom->getMapName(), AssetManager, &dude, &itemsInWorld, currentRoom);
#else
                    map.load(currentRoom->getMapName(), &dude, &itemsInWorld, currentRoom);
#endif
                    dude.setPosition(*map.getPlayerPos(currentPlayerEntryPoint));
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
                    updateWorld(dayLength/24 * 6);
                    calcDarknessValue();
                    dude.stopSleep();

                    if (darkness > 0.6f)
                    {
                        currentRoom->addConstantEnemy(Vector3D(371, 170, 0), 2);
                        map.addActor(currentRoom->getConstantEnemy(currentRoom->getConstantEnemyCount() - 1));
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

   
    //dude.update(DeltaTime, Keys, map, darkness, itemDB, path);
    updateWorld(DeltaTime);


    if (activeContainer)
    {
        void* data[] = {&dude};

        bool clickedOn = false;

        if (activeContainer->checkInput(pixelArtScale, DeltaTime, touches, &selectedItem, itemSelected, clickedOn, selectedItemPos, data))
        {
            return;
        }
    }

    void* data[] = {&dude, &itemDB}; 

    bool clickedInventory = false;
    dude.checkInventoryInput(pixelArtScale,
                             DeltaTime,
                             touches,
                             &selectedItem,
                             itemSelected,
                             clickedInventory,
                             selectedItemPos,
                             data);

    //picking up items with mouse--
    if (!itemSelected && touches.down.count())
    {
        for (int i = 0; i < map.getItemCount(); ++i)
        {
            ItemInstance* itm = map.getItem(i);
            
            if (itm->isRemoved())
            {
                continue;
            }

            if (!CollisionCircleCircle(itm->getPosition()->x + mapPosX, itm->getPosition()->y + mapPosY, 16,
                                      dude.getPos()->x + mapPosX, dude.getPos()->y + mapPosY, 100))
            {
                continue;
            }

            if (CollisionCircleCircle(itm->getPosition()->x * scale + mapPosX, itm->getPosition()->y * scale + mapPosY, 16,
                                      touches.down[0].x, touches.down[0].y, 4))
                    {
                        itemSelected = true;
                        selectedItem = itm;
                    }
        }
    }//----

    if (touches.move.count())
    { 
        if (itemSelected)
        {
            selectedItemPos = touches.move[0];
        }
    }

    if (touches.up.count())
    {
        if (interactWithFurniture((touches.up[0].x - mapPosX) / scale, (touches.up[0].y - mapPosY)/ scale))
        {
            return;
        }
        
        if (clickedInventory)
        {
            return;
        }

        if (itemSelected)
        {
            printf("adding selected item to map\n");
            const float newItemPosX = (selectedItemPos.x - mapPosX) / scale;
            const float newItemPosY = (selectedItemPos.y - mapPosY) / scale;

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


        if (recipeClicked != -1)
        {
            if (craftRecipeButton.isPointerOnTop(touches.up[0].x, touches.up[0].y))
            {
                Recipe* recipe = recipes.getRecipe(recipeClicked);
                recipeClicked = -1;
                dude.craftItem(recipe, itemDB);
                return;
            }
            else
            {
                recipeClicked = -1;
                return;
            }
        }

        
        if (handleCrafting(touches.up[0].x, touches.up[0].y, pixelArtScale))
        {
            return;
        }

        if (handleAttack((touches.up[0].x - mapPosX) / scale, (touches.up[0].y - mapPosY) / scale))
        {
            return;
        }

        Vector3D src = Vector3D(dude.getPos()->x, dude.getPos()->y + 39.0f, 0);
        Vector3D destination = Vector3D((touches.up[0].x - mapPosX) / scale, (touches.up[0].y - mapPosY) / scale, 0);
        dude.resetPathIndex();
        path.find(src, 
                  destination, 
                  map.getPolygonData(), 
                  map.getPolygonCount(), 
                  map.getAdditionalPathPoints(),
                  map.getAdditionalPathPointCount());
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
        Statistics::getInstance()->send(days * dayLength + worldTime - (dayLength / 24) * 10,
                                        time(0) - sessionStarted, false, statsPostRequest);
        sendStats = true;
        Statistics::getInstance()->reset();
    }

}
    
void Game::updateWorld(float deltaT)
{
    worldTime += deltaT;

    if (worldTime >= dayLength)
    {
        int daysPassed = worldTime / dayLength;
        days += daysPassed;
        worldTime = (worldTime - daysPassed * dayLength);
    }
    
    dude.update(deltaT, Keys, map, currentRoom, darkness, itemDB, recipes, furnitureDB, path);
    projectiles.update(deltaT, currentRoom, &map); 
    map.update(DeltaTime, &dude, pics);
}

void Game::editingLogic()
{
    if (touches.down.count())
    {
        
    }
    else if (touches.move.count())
    {
      
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
        path.destroy();
        mapGraph.destroy();
        mapGraph.generate(furnitureDB);
        mapGraph.findYardMapWidthHeight(yardMapWidth, yardMapHeight);
        printf("yard map size: %d %d\n", yardMapWidth, yardMapHeight);
        currentRoom = mapGraph.getRoot();
#ifdef __ANDROID__
        itemsInWorld.load("data/itemLocations.xml", AssetManager);
#else
        itemsInWorld.load("data/itemLocations.xml");
#endif
        gameMode = GAME;
        sessionStarted = time(0);
        dude.destroy();
#ifdef __ANDROID__
        map.load(currentRoom->getMapName(), AssetManager, &dude, &itemsInWorld, currentRoom);
#else
        map.load(currentRoom->getMapName(), &dude, &itemsInWorld, currentRoom);
#endif
        dude.init(*map.getPlayerPos(0), currentRoom, &map, ScreenWidth, ScreenHeight, pixelArtScale);
        dude.addDoubleClickCallbackForItems(&useItem);

        glClearColor(0.f, 0.0f, 0.0f, 0.0f);
        
        ignoreRegion = false;
        selectedItem = nullptr;
        itemSelected = false;
        selectedItemPos = Vector3D(0,0,0);
        activeContainer = nullptr;
        worldTime = dayLength/24 * 10;
        days = 0;
        clickOnItem = -1;
        stateInTheGame = FADEIN;
        fadeProgress = 0.f;
        recipeClicked = -1;
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

    activeContainer->draw(pics, itemDB, selectedItem, pixelArtScale, false);

}

void Game::drawRecipes(int scale)
{
    char buf[128];
    WriteText(10 * scale, 10 * scale, pics, 0, "Craft:", 0.8f * scale, 0.8f * scale);

    const float tileSize = 32.f;
    const float halfTileSize = tileSize / 2.f;

    int recipeIndex = 0;

    for (unsigned i = 0; i < recipes.getRecipeCount(); ++i)
    {

        Recipe* recipe = recipes.getRecipe(i);

        if (!recipe->craftable)
        {
            continue;
        }

        pics.draw(2, 
                  10 * scale, 
                  (tileSize + recipeIndex * (tileSize + 2)) * scale, 0, false, 0.25f * scale, 0.5 * scale);
        pics.draw(2, 
                  (10 + halfTileSize) * scale, 
                  (tileSize + recipeIndex * (tileSize + 2)) * scale, 1, false, 0.25f * scale, 0.5 * scale);

        pics.draw(4, 
                  10 * scale, 
                  (tileSize + recipeIndex * 34) * scale, 
                  recipe->indexOfItemToMake,
                  false,
                  scale,
                  scale);


        if (recipeClicked != -1 && recipeIndex == recipeClicked)
        {
            const float panelWidth = (recipe->ingredients.count() * 34 + 64) ;
            const float halfPanelWidth = panelWidth / 2.f;

            pics.draw(2, 
                      42 * scale,
                      (tileSize + recipeIndex * 34 - 17) * scale,
                      0, false, (halfPanelWidth / 64.f) * scale , 1.f * scale, 0, COLOR(1,1,1, 0.5));
            pics.draw(2, 
                      (42 + halfPanelWidth) * scale,
                      (32 + recipeIndex * 34 - 17) * scale,
                      1,
                      false, 
                      (halfPanelWidth / 64.f) * scale, 
                      1.f * scale, 0, COLOR(1,1,1, 0.5));

            for (unsigned j = 0; j < recipe->ingredients.count(); ++j)
            {
                pics.draw(4,
                          (10 + 34 + j * 32) * scale,
                          (32 + recipeIndex * 34 + 8 - 17) * scale,
                          recipe->ingredients[j].itemIndex,
                          false,
                          scale,
                          scale);
                sprintf(buf, "%d", recipe->ingredients[j].count);

                bool haveEnough = (dude.getItemCountByTypeIndex(recipe->ingredients[j].itemIndex) >= recipe->ingredients[j].count);

                COLOR countColor = (haveEnough) ? COLOR(1, 1, 1, 1) : COLOR(1, 0, 0, 1);

                WriteShadedText((10 + 34 + j * 32 + 8) * scale,
                          (32 + recipeIndex * 34 + 32 + 8 - 17) * scale,
                          pics, 0, buf, 0.8f * scale, 0.8f * scale, countColor, countColor);
            }

            craftRecipeButton.drawTextnPicture(pics, 20, 0, 0, "Make");
        }


        ++recipeIndex;
    }

}

void Game::drawDarkness(int scale)
{
    if (darkness <= 0.f)
    {
        return;
    }

    pics.drawBatch(&colorShader, &defaultShader, 666);



    //draw to FBO

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    //GLenum res = glCheckFramebufferStatus(fbo);
    glViewport(0, 0, 256, 256);
    glClear(GL_COLOR_BUFFER_BIT);


    map.drawLights(mapPosX, mapPosY, scale, pics);

    pics.draw(11,
              dude.pos.x * scale + mapPosX,
              (dude.pos.y - 20) * scale + mapPosY,
              0,
              true,
              scale, scale,
              0.f,
              COLOR(1,1,1,0.6),
              COLOR(1,1,1,0.6f));


    if (dude.isWeaponEquiped() == 12 && dude.getAmmoInWeaponCount()) //flashlight equiped
    {

        if (dude.getAnimationSubset() == 0)
        {
            pics.draw(15,
                    dude.pos.x * scale + mapPosX + ((dude.isFlipedX()) ? 89 * scale : -89 * scale),
                    dude.pos.y * scale + mapPosY - 20 * scale,
                    0,
                    false, (dude.isFlipedX()) ? -1.6 * scale : 1.6 * scale, 1.3 * scale);
        }
        else if (dude.getAnimationSubset() == 1)
        {

            pics.draw(7,
                    dude.pos.x * scale + mapPosX,
                    dude.pos.y * scale + mapPosY - 60,
                    0,
                    true,
                    1.8f * scale, 1.5f * scale,
                    0.f);

        }
        else
        {

            pics.draw(13,
                    dude.pos.x * scale + mapPosX + ((dude.isFlipedX()) ? 335 * scale : -335 * scale),
                    dude.pos.y * scale + mapPosY - 68 * scale,
                    0,
                    false, (dude.isFlipedX()) ? -1.3f * scale : 1.3f * scale, 1.3f * scale );
        }
    }


    pics.drawBatch(&colorShader, &defaultShader, 666);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, ScreenWidth, ScreenHeight);
    //printf("%d %d\n", ScreenWidth, ScreenHeight);

    int dark  = darknessShader.getUniformID("uDarkness");
    int light = darknessShader.getUniformID("uLight");

    darknessShader.use();
    glUniform1i(dark, 0);
    glUniform1i(light, 1);


    float verts[] = {0, 0, 
        ScreenWidth * 1.f, 0,
        ScreenWidth * 1.f, ScreenHeight * 1.f,
        0, 0,
        ScreenWidth * 1.f, ScreenHeight * 1.f,
        0, ScreenHeight * 1.f};

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


#ifdef WIN32
    wglActiveTexture(GL_TEXTURE0 + 0);
#else   
    glActiveTexture(GL_TEXTURE0 + 0);
#endif
    glBindTexture(GL_TEXTURE_2D, pics.getname(12));

#ifdef WIN32
    wglActiveTexture(GL_TEXTURE0 + 1);
#else 
    glActiveTexture(GL_TEXTURE0 + 1);
#endif

    glBindTexture(GL_TEXTURE_2D, pics.getname(fboTextureIndex)); 

    pics.drawVA(verts, uvs, colors, 12, 12, &darknessShader);
#ifdef WIN32
    wglActiveTexture(GL_TEXTURE0);
#else
    glActiveTexture(GL_TEXTURE0);
#endif
    //pics.draw(fboTextureIndex, 0, 0, 0);
}

void Game::calcDarknessValue()
{
    float sixOClock = dayLength/24 * 6;
    float eightOClock = dayLength/24 * 20;
    float daylightDuration = dayLength/24 * 14;
    darkness = (worldTime >= sixOClock && worldTime < eightOClock) ? 1.f - sinf(((worldTime - sixOClock) / daylightDuration) * M_PI) : 1.f;
}

void Game::doubleClickContainerItem(ItemInstance* item, void** dud)
{
    if (item)
    {
        if (item->isRemoved())
        {
            return;
        }

        Dude* d = (Dude*)dud[0];

        int freedSlot = d->findFreedInventorySlot();
        if (!d->isNoMorePlaceInBag(freedSlot))
        {
            d->addItemToInventory(item, freedSlot);
            Statistics::getInstance()->increaseItemAddition();
            item->setAsRemoved();
        }
    }
}

void Game::useItem(ItemInstance* item, void** data)
{
    if (item)
    {
        if (item->isRemoved())
        {
            return;
        }

        Dude* dud = (Dude*)data[0];
        ItemDatabase* db = (ItemDatabase*)data[1];

        if (dud)
        {
            dud->useItem(item, db);
        }

    }
}

bool Game::interactWithFurniture(float clickX, float clickY)
{
    Furniture * fur = map.getClickedFurniture(clickX, clickY, true);

    if (fur)
    {
        if (dude.isWeaponEquiped() == 19) //destroying furniture whith an axe
        {
            dude.activateMeleeAttack();
            return true;
        }

        if (fur->itemContainerIndex != -1 && (!activeContainer || !activeContainer->isActive()))
        {
            activeContainer = map.getItemContainer(fur->itemContainerIndex);
            activeContainer->setPosition(Vector3D(touches.up[0].x, touches.up[1].y, 0));
            activeContainer->setActive(true);
            activeContainer->setDoubleClickCallback(&doubleClickContainerItem);
            return true;
        }
        else if (fur->isBed)
        {
            dude.pos.x = fur->pos.x + 113.f;
            dude.pos.y = fur->pos.y + 67.f;
            dude.flipX(false);
            dude.goToSleep();
        }
    }

    return false;
}

void Game::centerCamera(float x, float y, int scale)
{
    const float mapWidth = map.getWidth() * 32.f * scale;
    const float mapHeight = map.getHeight() * 32.f * scale;

    if (mapWidth > ScreenWidth)
    {
        mapPosX = ScreenWidth / 2.f - dude.pos.x * scale;

        if (mapPosX > 0)
        {
            mapPosX = 0;
        }

        if (mapPosX + mapWidth < ScreenWidth)
        {
            mapPosX =  ScreenWidth - mapWidth;
        }
    }
    else
    {
        mapPosX = ScreenWidth / 2.f - mapWidth / 2.f;
    }
//----
    if (mapHeight > ScreenHeight)
    {

        mapPosY = ScreenHeight / 2.f - dude.pos.y * scale;

        if (mapPosY > 0)
        {
            mapPosY = 0;
        }

        if (mapPosY + mapHeight < ScreenHeight)
        {
            mapPosY = ScreenHeight - mapHeight;
        }
    }
    else
    {
        mapPosY = ScreenHeight / 2.f  - mapHeight / 2.f;
    }


}

void Game::drawPolygon(SPolygon* poly, int scale, ShaderProgram& shader, int method, COLOR c)
{
    DArray<float> vertices;
    DArray<float> colors;
    unsigned vertexCount = 0;

    for (unsigned long j = 0; j < poly->points.count(); ++j)
    {
        colors.add(c.r);
        colors.add(c.g);
        colors.add(c.b);
        colors.add(c.a);
        vertices.add(poly->points[j].x * scale + mapPosX);
        vertices.add(poly->points[j].y * scale + mapPosY);
        ++vertexCount;
    }

    int attribID = shader.getAttributeID("aPosition"); 
    int ColorAttribID = shader.getAttributeID("aColor");

    glVertexAttribPointer(attribID, 2, GL_FLOAT, GL_FALSE, 0, vertices.getData());
    glEnableVertexAttribArray(attribID);


    glVertexAttribPointer(ColorAttribID, 4, GL_FLOAT, GL_FALSE, 0, colors.getData());
    glEnableVertexAttribArray(ColorAttribID);

    glDrawArrays(method, 0, vertexCount);
    glDisableVertexAttribArray(ColorAttribID);
    glDisableVertexAttribArray(attribID);
    vertices.destroy();
    colors.destroy();
}

bool Game::handleAttack(float x, float y)
{
    int weaponEquiped = dude.isWeaponEquiped();
    
    if (weaponEquiped == -1)
    {
        return false;
    }

    ItemData* weaponInfo = itemDB.get(weaponEquiped);

    if (weaponInfo->imageIndex != 9 && weaponInfo->imageIndex != 19)
    {
        printf("nope\n");
        return false;
    }

    ActorContainer* actors = map.getActorContainer();


    for (unsigned i = 0; i < actors->getActorCount(); ++i)
    {
        Actor* actor = actors->getActor(i);

        if (actor->getType() != 1 || actor->isDead())
        {
            continue;
        }

        BoundingBox* bbox = actor->getCurrentTargetingBBox();


        if (CollisionCircleRectangle(x, y, 1.f,
                                     actor->pos.x + bbox->pos.x, 
                                     actor->pos.y + bbox->pos.y,
                                     bbox->size.x,
                                     bbox->size.y))
        {
            if (weaponInfo->imageIndex == 19) //axe - mellee
            {
                Vector3D dudePos = *dude.getPos();
                dudePos.y += 39.f;

                printf("SWING!\n");

                if (CollisionCircleCircle(dudePos.x, dudePos.y, 37,
                            actor->pos.x + actor->collisionBodyOffset.x,
                            actor->pos.y + actor->collisionBodyOffset.y,
                            actor->getCollisionBodyRadius()))
                {
                    dude.activateMeleeAttack();
                    return true;
                }
            }

        }
    }

    if (weaponInfo->imageIndex == 9) //slingshot - ranged
    {

        int ammoSlot = dude.hasItem(weaponInfo->ammoItemIndex);

        if (ammoSlot != -1)
        {
            Vector3D destination = Vector3D(x, y, 0);

            Vector3D diff = destination - dude.pos;
            diff.normalize();
            dude.setAnimationSubsetByDirectionVector(diff, false); 
            projectiles.addProjectile(dude.pos, destination);
            dude.removeItem(ammoSlot);
            dude.wearWeapon(-8.f);

            return true;
        }
    }
    

    return false;   
}

bool Game::handleCrafting(float x, float y, int scale)
{
    for (unsigned i = 0; i < recipes.getRecipeCount(); ++i)
    {
        if (x > 10 * scale && x < 42 * scale &&
                y > (32 + i * 34) * scale && y < (64 + i * 34) * scale)
        {
            recipeClicked = i;
            craftRecipeButton.init((recipes.getRecipe(i)->ingredients.count() * 32 + 10 + 4 + 32) * scale,
                                   (i * 34 + 32 + 7 - 17) * scale , 50 * scale, 50 * scale);
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
