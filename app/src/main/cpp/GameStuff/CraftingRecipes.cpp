#include "CraftingRecipes.h"

#include "../Xml.h"


#ifndef __ANDROID__
void CraftingRecipes::load(const char* filename)
#else
void CraftingRecipes::load(const char* filename, AAssetManager* assman)
#endif
{
    Xml recipeFile;
#ifdef __ANDROID__
    recipeFile.load(filename, assman);
#else
    recipeFile.load(filename);
#endif

    char buffer[100];
    XmlNode* recipesNode = recipeFile.root.getNode(L"Recipes");

    for (unsigned i = 0; i < recipesNode->childrenCount(); ++i)
    {
        XmlNode* recipeNode = recipesNode->getNode(i);
        
        if (recipeNode)
        {
            Recipe recipe;
   
            for (unsigned a = 0; a < recipeNode->attributeCount(); ++a)
            {
                XmlAttribute* at = recipeNode->getAttribute(a);
                sprintf(buffer, "%ls", at->getName());

                if (strcmp(buffer, "resultItemIndex") == 0)
                {
                    sprintf(buffer, "%ls", at->getValue());
                    recipe.indexOfItemToMake = atoi(buffer);
                }
                else if (strcmp(buffer, "craftable") == 0)
                {
                    sprintf(buffer, "%ls", at->getValue());
                    recipe.craftable = atoi(buffer);
                }
                else if (strcmp(buffer, "resultFurnitureIndex") == 0)
                {
                    sprintf(buffer, "%ls", at->getValue());
                    recipe.indexOfFurnitureToMake = atoi(buffer);
                }

            }

            

            for (unsigned j = 0; j < recipeNode->childrenCount(); ++j)
            {
                XmlNode* ingredientNode = recipeNode->getNode(j);

                if (ingredientNode)
                {
                    Ingredient ingredient;

                    for (unsigned k = 0; k < ingredientNode->attributeCount(); ++k)
                    {
                        XmlAttribute* at = ingredientNode->getAttribute(k);
                        sprintf(buffer, "%ls", at->getName());

                        if (strcmp(buffer, "itemIndex") == 0)
                        {
                            sprintf(buffer, "%ls", at->getValue());
                            ingredient.itemIndex = atoi(buffer);
                        }
                        else if (strcmp(buffer, "count") == 0)
                        {
                            sprintf(buffer, "%ls", at->getValue());
                            ingredient.count = atoi(buffer);
                        }

                    }

                    recipe.ingredients.add(ingredient);
                }

            }

            recipes.add(recipe);

        }
    }

    recipeFile.destroy();


}

unsigned CraftingRecipes::getRecipeCount()
{
    return recipes.count();
}

Recipe*  CraftingRecipes::getRecipe(unsigned index)
{
    if (index < recipes.count())
    {
        return &recipes[index];
    }

    return nullptr;
}

Recipe*  CraftingRecipes::getRecipeByFurnitureIndex(unsigned index)
{
    for (unsigned i = 0; i < recipes.count(); ++i)
    {
        Recipe* r = &recipes[i];

        if (r->indexOfFurnitureToMake == (int)index)
        {
            return r;
        }
    }

    return nullptr;
}

void CraftingRecipes::destroy()
{
    for (unsigned i = 0; i < recipes.count(); ++i)
    {
        recipes[i].ingredients.destroy();
    }

    recipes.destroy();
}

