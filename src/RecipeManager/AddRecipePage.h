#ifndef ADD_RECIPE_PAGE_H
#define ADD_RECIPE_PAGE_H

#include "../../vendor/page.h"
#include "../../vendor/sysfunc/out.h"
#include "Recipe.h"
#include <vector>

using namespace std;

class AddRecipePage : public Page {
private:
    vector<Recipe>* recipes;

    void boxOpen() { out.coutln("+-----------------------------------------+"); }
    void boxClose() { out.coutln("+-----------------------------------------+"); }

    void printRecipe(const Recipe& r) {
        boxOpen();
        out.br();
        out.coutln("Recipe: " + r.name);
        out.br();
        out.coutln("Ingredients: ");
        for (const auto& ing : r.ingredients) {
            out.coutln("- " + ing);
        }
        out.br();
        out.coutln(r.instructions);
        out.br();
        boxClose();
    }

public:
    AddRecipePage(vector<Recipe>* recipesRef)
        : Page("Add Recipe", "Add Recipe", {"Recipe Manager"}), recipes(recipesRef) {}

    void schema() override {
        string recipeName = out.inputs("Enter recipe name: ");
        string ingStr = out.inputs("Enter ingredients (comma-separated): ");
        string instr = out.inputs("Enter instructions: ");
        vector<string> ings = RecipeUtils::splitCsv(ingStr);

        while (true) {
            out.br();
            printRecipe(Recipe{recipeName, ings, instr});
            bool sure = out.inputYesNo("Are you sure? (y/n): ");
            if (sure) {
                recipes->push_back(Recipe{recipeName, ings, instr});
                out.coutln("Recipe '" + recipeName + "' added successfully!");
                break;
            }
            out.br();
            out.coutln("What do you want to change:");
            out.displayChoices({"Recipe name", "Ingredients", "instructions", "None"});
            int c = out.inputi("Enter your choice: ");
            if (c == 1) {
                recipeName = out.inputs("Recipe name: ");
            } else if (c == 2) {
                ingStr = out.inputs("Ingredients (comma-separated): ");
                ings = RecipeUtils::splitCsv(ingStr);
            } else if (c == 3) {
                instr = out.inputs("instructions: ");
            }
        }
    }
};

#endif // ADD_RECIPE_PAGE_H
