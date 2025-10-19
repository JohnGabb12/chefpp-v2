#ifndef DELETE_RECIPE_PAGE_H
#define DELETE_RECIPE_PAGE_H

#include "../../vendor/page.h"
#include "../../vendor/sysfunc/out.h"
#include "Recipe.h"
#include <vector>

using namespace std;

class DeleteRecipePage : public Page {
private:
    vector<Recipe>* recipes;

public:
    DeleteRecipePage(vector<Recipe>* recipesRef)
        : Page("Delete Recipe", "Delete Recipe", {"Recipe Manager"}), recipes(recipesRef) {}

    void schema() override {
        if (recipes->empty()) {
            out.coutln("No recipes to delete.");
            return;
        }

        // Show simple list
        for (int i = 0; i < static_cast<int>(recipes->size()); ++i) {
            out.coutln(to_string(i+1) + ". " + (*recipes)[i].name);
        }

        int idx = out.inputi("Which recipe to delete?: ") - 1;
        if (idx < 0 || idx >= static_cast<int>(recipes->size())) {
            out.coutln("Invalid selection.");
            return;
        }

        const string recipeName = (*recipes)[idx].name;
        if (out.inputYesNo("Are you sure you want to delete '" + recipeName + "'? (y/n): ")) {
            recipes->erase(recipes->begin() + idx);
            out.coutln("Recipe '" + recipeName + "' deleted successfully!");
        } else {
            out.coutln("Deletion cancelled.");
        }
    }
};

#endif // DELETE_RECIPE_PAGE_H
