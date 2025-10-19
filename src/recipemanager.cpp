#include "../vendor/page.h"
#include "../vendor/sysfunc/out.h"
#include "RecipeManager/Recipe.h"
#include "RecipeManager/AddRecipePage.h"
#include "RecipeManager/ViewRecipesPage.h"
#include "RecipeManager/SearchRecipePage.h"
#include "RecipeManager/DeleteRecipePage.h"
#include "RecipeManager/CheckIngredientsPage.h"
#include <vector>

using namespace std;

class RecipeManager : public Page
{
private:
    vector<Recipe> recipes;
    const string filename = "data/recipes.json";

public:
    RecipeManager() : Page("Recipe Manager", "RECIPE MANAGER", {}) {
        recipes = Recipe::loadFromFile(filename);
    }

    void schema() override
    {
        out.coutln("Options:");
        out.displayChoices({"Add Recipe", "View Recipes", "Search Recipe", "Delete Recipe", "Check Ingredients", "Back"});
        out.br();
        int choice = out.inputi("Enter your choice: ");

        switch (choice)
        {
        case 1: {
            AddRecipePage addPage(&recipes);
            addPage.init();
            if (!Recipe::saveToFile(filename, recipes)) {
                out.coutln("Error: Could not save recipes to file.");
            }
            break;
        }
        case 2: {
            ViewRecipesPage viewPage(&recipes);
            viewPage.init();
            break;
        }
        case 3: {
            SearchRecipePage searchPage(&recipes);
            searchPage.init();
            break;
        }
        case 4: {
            DeleteRecipePage deletePage(&recipes);
            deletePage.init();
            if (!Recipe::saveToFile(filename, recipes)) {
                out.coutln("Error: Could not save recipes to file.");
            }
            break;
        }
        case 5: {
            CheckIngredientsPage checkPage(&recipes);
            checkPage.init();
            break;
        }
        case 6:
            return;
        }
        this->init();
    }
};
