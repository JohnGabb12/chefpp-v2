#ifndef SEARCH_RECIPE_PAGE_H
#define SEARCH_RECIPE_PAGE_H

#include "../../vendor/page.h"
#include "../../vendor/sysfunc/out.h"
#include "Recipe.h"
#include "ViewRecipesPage.h"
#include <vector>

using namespace std;

class SearchRecipePage : public Page {
private:
    vector<Recipe>* recipes;

    vector<int> searchByName(const string& q) {
        vector<int> idx;
        string qq = RecipeUtils::toLower(q);
        for (int i = 0; i < static_cast<int>(recipes->size()); ++i) {
            if (RecipeUtils::toLower((*recipes)[i].name).find(qq) != string::npos) {
                idx.push_back(i);
            }
        }
        return idx;
    }

public:
    SearchRecipePage(vector<Recipe>* recipesRef)
        : Page("Search Recipe", "Search Recipe", {"Recipe Manager"}), recipes(recipesRef) {}

    void schema() override {
        string query = out.inputs("Search: ");
        auto indices = searchByName(query);

        if (indices.empty()) {
            out.coutln("No recipes found matching '" + query + "'.");
            return;
        }

        // Use ViewRecipesPage to display results
        ViewRecipesPage viewPage(recipes, indices);
        viewPage.setLabel("Search Results");
        viewPage.init();
    }
};

#endif // SEARCH_RECIPE_PAGE_H
