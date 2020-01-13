# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")

add_library(app-glue STATIC
            ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

add_library(game SHARED

             src/main/cpp/game_android.cpp
             src/main/cpp/Game.cpp
             src/main/cpp/TextureLoader.cpp

             src/main/cpp/Vectors.cpp

             src/main/cpp/Particles2D.cpp
             src/main/cpp/Utils.cpp
             src/main/cpp/Xml.cpp
             src/main/cpp/SysConfig.cpp
             src/main/cpp/Entity.cpp

             src/main/cpp/Matrix.cpp
             src/main/cpp/Shader.cpp
             src/main/cpp/ShaderProgram.cpp
             src/main/cpp/rm2.cpp

             src/main/cpp/Usefull.cpp
             src/main/cpp/Image.cpp


             src/main/cpp/gui/Text.cpp
             src/main/cpp/gui/Button.cpp
             src/main/cpp/gui/OnScreenBoard.cpp
             src/main/cpp/audio/AndroidSoundSystem.cpp

             src/main/cpp/GameStuff/Dude.cpp
             src/main/cpp/GameStuff/GameMap.cpp
             src/main/cpp/GameStuff/GameMapGraph.cpp
             src/main/cpp/GameStuff/ItemInstance.cpp
             src/main/cpp/GameStuff/ItemContainer.cpp
             src/main/cpp/GameStuff/Actor.cpp
             src/main/cpp/GameStuff/Rat.cpp
             src/main/cpp/GameStuff/Ghost.cpp
             src/main/cpp/GameStuff/ActorContainer.cpp
             src/main/cpp/GameStuff/GlobalItemList.cpp
             src/main/cpp/GameStuff/ItemDatabase.cpp
             src/main/cpp/GameStuff/CraftingRecipes.cpp
             src/main/cpp/GameStuff/Dude.cpp
             src/main/cpp/FindPath.cpp

             )



find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )



target_include_directories(game PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           ${ANDROID_NDK}/sources/android/native_app_glue)




target_link_libraries(
                       game
                       android
                       EGL
                       GLESv2
                       app-glue
                       OpenSLES


                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )
