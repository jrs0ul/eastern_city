# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")

set(SRCDIR ../src/)

add_library(app-glue STATIC
            ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

add_library(game SHARED

             ${SRCDIR}game_android.cpp
             ${SRCDIR}Game.cpp
             ${SRCDIR}TextureLoader.cpp

             ${SRCDIR}Vectors.cpp

             ${SRCDIR}Particles2D.cpp
             ${SRCDIR}OSTools.cpp
        ${SRCDIR}Xml.cpp
        ${SRCDIR}SysConfig.cpp
        ${SRCDIR}Entity.cpp

        ${SRCDIR}Matrix.cpp
        ${SRCDIR}Shader.cpp
        ${SRCDIR}ShaderProgram.cpp
        ${SRCDIR}rm2.cpp

        ${SRCDIR}MathTools.cpp
        ${SRCDIR}Image.cpp


        ${SRCDIR}gui/Text.cpp
        ${SRCDIR}gui/Button.cpp
        ${SRCDIR}gui/OnScreenBoard.cpp
        ${SRCDIR}audio/AndroidSoundSystem.cpp

        ${SRCDIR}GameStuff/map/GameMap.cpp
        ${SRCDIR}GameStuff/map/GameMapGraph.cpp
        ${SRCDIR}GameStuff/FurnitureData.cpp
        ${SRCDIR}GameStuff/Projectile.cpp
        ${SRCDIR}GameStuff/ProjectileContainer.cpp
        ${SRCDIR}GameStuff/map/Room.cpp
        ${SRCDIR}GameStuff/Statistics.cpp
        ${SRCDIR}GameStuff/ItemInstance.cpp
        ${SRCDIR}GameStuff/ItemContainer.cpp
        ${SRCDIR}GameStuff/actors/Actor.cpp
        ${SRCDIR}GameStuff/actors/Rat.cpp
        ${SRCDIR}GameStuff/actors/Ghost.cpp
        ${SRCDIR}GameStuff/actors/Dude.cpp
        ${SRCDIR}GameStuff/actors/Bear.cpp
        ${SRCDIR}GameStuff/ActorContainer.cpp
        ${SRCDIR}GameStuff/GlobalItemList.cpp
        ${SRCDIR}GameStuff/ItemDatabase.cpp
        ${SRCDIR}GameStuff/CraftingRecipes.cpp
        ${SRCDIR}GameStuff/Furniture.cpp
        ${SRCDIR}FindPath.cpp

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
