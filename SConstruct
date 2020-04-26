import os
import sys

OBJ_DIR = os.path.join(os.getcwd(), 'obj/');
env = Environment(ENV=os.environ,
                  #CCFLAGS=['-ggdb', '-O0'],
                  CCFLAGS=['-O3','-Wall'],
                  OBJPREFIX = OBJ_DIR)


code = [
        "src/GameStuff/Furniture.cpp",
        "src/GameStuff/FurnitureData.cpp",
        "src/GameStuff/ItemContainer.cpp",
        "src/GameStuff/CraftingRecipes.cpp",
        "src/GameStuff/map/GameMap.cpp",
        "src/GameStuff/map/Room.cpp",
        "src/GameStuff/map/GameMapGraph.cpp",
        "src/GameStuff/ItemInstance.cpp",
        "src/GameStuff/GlobalItemList.cpp",
        "src/GameStuff/ItemDatabase.cpp",
        "src/GameStuff/ActorContainer.cpp",
        "src/GameStuff/actors/Actor.cpp",
        "src/GameStuff/actors/Dude.cpp",
        "src/GameStuff/actors/Rat.cpp",
        "src/GameStuff/actors/Ghost.cpp",
        "src/GameStuff/actors/Bear.cpp",
        "src/GameStuff/Statistics.cpp",
        "src/GameStuff/Projectile.cpp",
        "src/GameStuff/ProjectileContainer.cpp",
        "src/GameStuff/Consts.cpp",
        "src/FindPath.cpp",
        "src/MathTools.cpp",
        "src/SDLVideo.cpp",
        "src/Extensions.cpp",
        "src/gui/OnScreenBoard.cpp",
        "src/audio/OggStream.cpp",
        "src/audio/SoundSystem.cpp",
        "src/Xml.cpp",
        "src/Particles2D.cpp",
        "src/Image.cpp",
        "src/Shader.cpp",
        "src/ShaderProgram.cpp",
        "src/TextureLoader.cpp",
        "src/SysConfig.cpp",
        "src/OSTools.cpp",
        "src/Matrix.cpp",
        "src/Vectors.cpp",
        "src/gui/Button.cpp",
        "src/gui/Text.cpp",
        "src/Entity.cpp",
        "src/Game.cpp",
        "src/mainer.cpp"
        ]

#Windblows
if sys.platform == 'win32':
    Tool('mingw')(env)
    env.Append(LIBS = ['mingw32','SDL2main', 'SDL2', 'OpenGL32',
		        'imagehlp', 'dxguid', 'dxerr8',
			'oleaut32', 'shell32', 'version', 'uuid',
                       'openal32', 'vorbisfile', 'ole32', 'imm32', 'user32', 'gdi32', 'winmm', 'dinput8', 'vorbis', 'ogg', 'curl'])
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

