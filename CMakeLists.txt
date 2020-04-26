cmake_minimum_required(VERSION 3.1)

project(EasternCity LANGUAGES CXX)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(easterncity 
                    src/mainer.cpp
                    src/GameStuff/Furniture.cpp
                    src/GameStuff/FurnitureData.cpp
                    src/GameStuff/ItemContainer.cpp
                    src/GameStuff/CraftingRecipes.cpp
                    src/GameStuff/map/GameMap.cpp
                    src/GameStuff/map/Room.cpp
                    src/GameStuff/map/GameMapGraph.cpp
                    src/GameStuff/ItemInstance.cpp
                    src/GameStuff/GlobalItemList.cpp
                    src/GameStuff/ItemDatabase.cpp
                    src/GameStuff/ActorContainer.cpp
                    src/GameStuff/actors/Actor.cpp
                    src/GameStuff/actors/Dude.cpp
                    src/GameStuff/actors/Rat.cpp
                    src/GameStuff/actors/Ghost.cpp
                    src/GameStuff/actors/Bear.cpp
                    src/GameStuff/Statistics.cpp
                    src/GameStuff/Projectile.cpp
                    src/GameStuff/ProjectileContainer.cpp
                    src/GameStuff/Consts.cpp
                    src/FindPath.cpp
                    src/MathTools.cpp
                    src/SDLVideo.cpp
                    src/Extensions.cpp
                    src/gui/OnScreenBoard.cpp
                    src/audio/OggStream.cpp
                    src/audio/SoundSystem.cpp
                    src/Xml.cpp
                    src/Particles2D.cpp
                    src/Image.cpp
                    src/Shader.cpp
                    src/ShaderProgram.cpp
                    src/TextureLoader.cpp
                    src/SysConfig.cpp
                    src/OSTools.cpp
                    src/Matrix.cpp
                    src/Vectors.cpp
                    src/gui/Button.cpp
                    src/gui/Text.cpp
                    src/Entity.cpp
                    src/Game.cpp
                    )

target_link_libraries(easterncity 
                        SDL2
                        GL
                        openal
                        vorbisfile
                        ogg
                        pthread
                        curl
                     )

