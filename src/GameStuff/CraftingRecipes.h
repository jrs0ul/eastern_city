#ifndef CRAFTING_RECIPES_H
#define CRAFTING_RECIPES_H

#include "../DArray.h"
#ifdef __ANDROID__
    #include <android/asset_manager.h>
#endif


struct Ingredient
{
    int itemIndex;
    int count;
};

struct Recipe
{
    DArray<Ingredient> ingredients;
    int indexOfItemToMake;
    int indexOfFurnitureToMake;
    bool craftable;

    Recipe()
    : indexOfItemToMake(-1)
    , indexOfFurnitureToMake(-1)
    , craftable(false)
    {
    }
};


class CraftingRecipes
{
public:
#ifndef __ANDROID__
    void     load(const char* filename);
#else
    void     load(const char* filename, AAssetManager* assman);
#endif
    unsigned getRecipeCount();
    Recipe*  getRecipe(unsigned index);
    Recipe*  getRecipeByFurnitureIndex(unsigned index);
    void     destroy();

private:
    DArray<Recipe> recipes;
};


#endif //CRAFTING_RECIPES
