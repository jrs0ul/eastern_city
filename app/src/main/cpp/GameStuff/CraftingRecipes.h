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
};


class CraftingRecipes
{
public:
#ifndef __ANDROID__
    void     load(const char* filename);
#else
    void     load(const char* filename, AssetManager& assman);
#endif
    unsigned getRecipeCount();
    Recipe*  getRecipe(unsigned index);
    void     destroy();

private:
    DArray<Recipe> recipes;
};


#endif //CRAFTING_RECIPES
