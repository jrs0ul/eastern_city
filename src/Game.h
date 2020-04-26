#ifndef _CPP_SINGLETON
#define _CPP_SINGLETON

#ifndef __ANDROID__
#include "audio/SoundSystem.h"
#include "audio/OggStream.h"
#else
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#include "audio/AndroidSoundSystem.h"
#endif
#include "Vectors.h"
#include "TouchData.h"
#include "gui/Button.h"
#include "SysConfig.h"
#include "ShaderProgram.h"
#include "Matrix.h"
#include "FindPath.h"
#include "GameStuff/ProjectileContainer.h"
#include "GameStuff/ItemDatabase.h"
#include "GameStuff/FurnitureData.h"
#include "GameStuff/CraftingRecipes.h"
#include "GameStuff/GlobalItemList.h"
#include "GameStuff/map/GameMap.h"
#include "GameStuff/map/GameMapGraph.h"
#include "GameStuff/ActorContainer.h"


#ifndef __IPHONEOS__
//   #define _DEBUG_ 1
#endif


const float posX = 45;
const float posY = 5;
const float fieldWidth = 3.f;
const float fieldHeight = 2.f;


enum GameModes
{
    TITLE,
    GAME,
    DEFEAT
};

enum GameStates
{
    GAMEPLAY,
    FADEOUT,
    FADEIN
};

//============================================
class Game
{

public:
  
    Game();

    void loadConfig();
    void Render2D();
    void GameLoop();

    void DrawDebugText();
    int  FPS();
    
    
    void init();
    void logic();
    void render();
    void destroy();
    void onBack();

private:

    //scenes rendering
    void renderGame();
    void renderTitle();
    void renderDefeat();

    //scenes logic
    void gameLogic();
    void updateWorld(float deltaTime);
    void titleLogic();
    void defeatLogic();
    //------------------

    void drawActiveContainer();
    void drawRecipes(int scale);
    void drawDarkness(int scale);
    void calcDarknessValue();

    static void doubleClickContainerItem(ItemInstance*, void**);
    static void useItem(ItemInstance*, void**);

    bool interactWithFurniture(float clickX, float clickY);

    void centerCamera(float x, float y, int scale);
    void createEnemies();
    void drawPolygon(SPolygon* poly, int scale, ShaderProgram& shader, int method = GL_LINE_STRIP, COLOR c = COLOR(0,0,1,1));

    bool handleAttack(float x, float y);
    bool handleCrafting(float x, float y, int scale);
    

    void LoadShader(ShaderProgram* shader, const char* name);



    void restartGame();



public:
    GameModes           gameMode;
#ifdef __ANDROID__
    AAssetManager* AssetManager;
#endif


    int ScreenWidth;
    int ScreenHeight;
    bool windowed;
    int pixelArtScale;

    char DocumentPath[255];

    unsigned char Keys[20];
    unsigned char OldKeys[20];
    Vector3D gamepad;

    char fileName[255];


#ifndef __ANDROID__
    OggStream music;
#endif
    
    PicsContainer pics;
    
    long TimeTicks;
    float DeltaTime;
    float Accumulator;
    float DT;
    long tick;


    
    int MouseXOld, MouseYOld, MouseX, MouseY, MouseButton, MouseButtonOld;
    
    
    Vector3D oldMoveTouch;
    
    char Key, OldKey;
    
    
    unsigned globalKEY;

    bool sendStats;
    char statsPostRequest[666];

    bool useAccel;
    Vector3D _acceleration;
    Vector3D firstTouch;
    Vector3D pos;

    
    SystemConfig             sys;
    
    TouchData                touches;
    bool                     Works;
    int                      DebugMode;


private:

    float                OrthoMatrix[16];
    float                ViewMatrix[16];
    FlatMatrix           pushed;


    ShaderProgram        defaultShader;
    ShaderProgram        colorShader;
    ShaderProgram        darknessShader;


    Dude* dude;
    ProjectileContainer projectiles;
    GameMap map;
    float mapPosX;
    float mapPosY;
    FindPath    path;
    bool    ignoreRegion;

    ItemDatabase itemDB;
    FurnitureDatabase furnitureDB;
    CraftingRecipes recipes;
    GlobalItemList itemsInWorld;

    GameMapGraph   mapGraph;
    int yardMapWidth;
    int yardMapHeight;
    Room* currentRoom;

    ItemContainer* activeContainer;

    ItemInstance* selectedItem;
    bool itemSelected;
    Vector3D selectedItemPos;

    int recipeClicked;
    Button craftRecipeButton;

    int clickOnItem;

    float worldTime;
    float darkness;
    int days;

    time_t sessionStarted;

    unsigned fbo;
    unsigned fboTexture;
    int fboTextureIndex;

    GameStates stateInTheGame;
    float fadeProgress;
    unsigned currentPlayerEntryPoint;

};


#endif //_CPP_SINGLETON

