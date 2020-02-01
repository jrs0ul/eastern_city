import os
import sys

OBJ_DIR = os.path.join(os.getcwd(), 'obj/');
env = Environment(ENV=os.environ,
                  #CCFLAGS=['-ggdb', '-O0'],
                  CCFLAGS=['-O3','-Wall'],
                  OBJPREFIX = OBJ_DIR)


code = [
        "app/src/main/cpp/GameStuff/GameMap.cpp",
        "app/src/main/cpp/GameStuff/Furniture.cpp",
        "app/src/main/cpp/GameStuff/ItemContainer.cpp",
        "app/src/main/cpp/GameStuff/CraftingRecipes.cpp",
        "app/src/main/cpp/GameStuff/GameMapGraph.cpp",
        "app/src/main/cpp/GameStuff/ItemInstance.cpp",
        "app/src/main/cpp/GameStuff/GlobalItemList.cpp",
        "app/src/main/cpp/GameStuff/ItemDatabase.cpp",
        "app/src/main/cpp/GameStuff/ActorContainer.cpp",
        "app/src/main/cpp/GameStuff/Actor.cpp",
        "app/src/main/cpp/GameStuff/Dude.cpp",
        "app/src/main/cpp/GameStuff/Rat.cpp",
        "app/src/main/cpp/GameStuff/Ghost.cpp",
        "app/src/main/cpp/GameStuff/Statistics.cpp",
        "app/src/main/cpp/FindPath.cpp",
        "app/src/main/cpp/Useful.cpp",
        "app/src/main/cpp/SDLVideo.cpp",
        "app/src/main/cpp/Extensions.cpp",
        "app/src/main/cpp/gui/OnScreenBoard.cpp",
        "app/src/main/cpp/audio/OggStream.cpp",
        "app/src/main/cpp/audio/SoundSystem.cpp",
        "app/src/main/cpp/Xml.cpp",
        "app/src/main/cpp/Particles2D.cpp",
        "app/src/main/cpp/Image.cpp",
        "app/src/main/cpp/Shader.cpp",
        "app/src/main/cpp/Threads.cpp",
        "app/src/main/cpp/ShaderProgram.cpp",
        "app/src/main/cpp/TextureLoader.cpp",
        "app/src/main/cpp/SysConfig.cpp",
        "app/src/main/cpp/Utils.cpp",
        "app/src/main/cpp/Matrix.cpp",
        "app/src/main/cpp/Vectors.cpp",
        "app/src/main/cpp/gui/Button.cpp",
        "app/src/main/cpp/gui/Text.cpp",
        "app/src/main/cpp/Entity.cpp",
        "app/src/main/cpp/Game.cpp",
        "app/src/main/cpp/mainer.cpp"
        ]

#Windblows
if sys.platform == 'win32':
    Tool('mingw')(env)
    env.Append(LIBS = ['mingw32','SDL2main', 'SDL2', 'OpenGL32',
		        'imagehlp', 'dxguid', 'dxerr8',
			'oleaut32', 'shell32', 'version', 'uuid',
                       'openal32', 'vorbisfile', 'ole32', 'imm32', 'user32', 'gdi32', 'winmm', 'dinput8', 'vorbis', 'ogg'])
else: #Mac OS X
    if sys.platform == 'darwin':
        env['FRAMEWORKS'] = ['Cocoa', 'SDL', 'OPENGL', 'OpenAL','vorbis', 'Ogg']
        env.Append(LIBS = ['iconv', 'pthread'])
        env.Append(CCFLAGS = ['-arch', 'i386'])
        env.Object('src/osx/SDLMain.m', OBJPREFIX = OBJ_DIR,
                   CCFLAGS = ['-arch', 'i386', '-I/Library/Frameworks/SDL.framework/Headers/'])
        code.append('obj/SDLMain.o')
        env.Append(LINKFLAGS = ['-arch', 'i386'])
    else: #Linux
        env.Append(LIBS = ['SDL2','GL', 'openal', 'vorbisfile',
                           'ogg', 'pthread', 'curl'])


env.Program(target = "empty", 
            source = code
            )

