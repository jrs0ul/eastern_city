#ifndef _CPP_SINGLETON
#define _CPP_SINGLETON

#include <ctime>
#ifndef __ANDROID__
#include "audio/SoundSystem.h"
#include "audio/OggStream.h"
#else
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#include "audio/AndroidSoundSystem.h"
#endif
#include "TextureLoader.h"
#include "Vectors.h"
#include "TouchData.h"
#include "gui/Text.h"
#include "gui/Button.h"
#include "SysConfig.h"
#include "Utils.h"
#include "Usefull.h"
#include "Particles2D.h"
#include "Entity.h"
#include "ShaderProgram.h"
#include "rm2.h"
#include "FindPath.h"
#include "GameStuff/Dude.h"
#include "GameStuff/ItemDatabase.h"
#include "GameStuff/CraftingRecipes.h"
#include "GameStuff/GlobalItemList.h"
#include "GameStuff/GameMap.h"
#include "GameStuff/Rat.h"
#include "GameStuff/GameMapGraph.h"
#include "GameStuff/ActorContainer.h"



#define SCREEN_WIDTH  850
#define SCREEN_HEIGHT 480



#ifndef __IPHONEOS__
//   #define _DEBUG_ 1
#endif


const float posX = 45;
const float posY = 5;
const float fieldWidth = 3.f;
const float fieldHeight = 2.f;
const float scale = 100.f;
const float TouchScreenJoystickRadius = 45.f;
const float TouchScreenJoystickX = 90;
const float TouchScreenJoystickY = 400;


enum GameModes
{
    TITLE,
    GAME,
    EDITING
};


//============================================
class Game
{

public:
  
#ifdef __ANDROID__
    AAssetManager* AssetManager;
#endif



    int ScreenWidth;
    int ScreenHeight;
    bool windowed;

    char DocumentPath[255];

    unsigned char Keys[20];
    unsigned char OldKeys[20];
    Vector3D gamepad;

    char fileName[255];


    SoundSystem ss;
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
    
    
    
    bool useAccel;
    Vector3D _acceleration;
    Vector3D firstTouch;
    Vector3D pos;

    
    SystemConfig             sys;
    
    TouchData                touches;
    bool                     Works;
    int                      DebugMode;

    void Render2D();
    void GameLoop();

    void DrawDebugText();
    int  FPS();
    
    Game()
    {


#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
        ScreenWidth = 850;
        ScreenHeight = 480;
#else
        ScreenWidth = 850;
        ScreenHeight = 480;
        windowed = false;
#endif

        Accumulator = 0;
        DT = 1000.0f/60.0f/1000.0f;
        oldMoveTouch = Vector3D(-1, -1, 0);
        Works = true;
        tick = 0;
        TimeTicks = 0;

        DebugMode = 0;
    }
    
    void init();
    void loadConfig();
    void logic();
    void render();
    void destroy();
    void onBack();

private:

    void renderGame();
    void renderEditing();
    void renderTitle();

    void gameLogic();
    void editingLogic();
    void titleLogic();

    void drawActiveContainer();
    void drawDarkness();

    void centerCamera(float x, float y);
    void createEnemies();

    bool handleShooting(float x, float y);
    bool handleCrafting(float x, float y);
    

    void LoadShader(ShaderProgram* shader, const char* name);



    void restartGame();



public:
    GameModes           gameMode;
private:

    float                OrthoMatrix[16];
    float                ViewMatrix[16];
    FlatMatrix           pushed;


    ShaderProgram        defaultShader;
    ShaderProgram        colorShader;
    ShaderProgram        darknessShader;


    Dude dude;
    ActorContainer actors;
    GameMap map;
    float mapPosX;
    float mapPosY;
    Path    path;
    bool    ignoreRegion;

    ItemDatabase itemDB;
    CraftingRecipes recipes;
    GlobalItemList itemsInWorld;

    GameMapGraph   mapGraph;
    Room* currentRoom;

    ItemContainer* activeContainer;

    ItemInstance* selectedItem;
    bool itemSelected;
    Vector3D selectedItemPos;

    int clickOnItem;

    float worldTime;
    int days;

    unsigned fbo;
    unsigned fboTexture;
    int fboTextureIndex;

};


#endif //_CPP_SINGLETON

