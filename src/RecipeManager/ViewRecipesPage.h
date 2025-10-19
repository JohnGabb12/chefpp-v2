#ifndef VIEW_RECIPES_PAGE_H
#define VIEW_RECIPES_PAGE_H

#include "../../vendor/page.h"
#include "../../vendor/sysfunc/out.h"
#include "Recipe.h"
#include <vector>
#include <algorithm>

using namespace std;

class ViewRecipesPage : public Page {
private:
    vector<Recipe>* recipes;
    vector<int> idxMap;

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
    ViewRecipesPage(vector<Recipe>* recipesRef, const vector<int>& indices = {})
        : Page("View Recipes", "View Recipes", {"Recipe Manager"}), recipes(recipesRef), idxMap(indices) {
        if (idxMap.empty()) {
            for (int i = 0; i < static_cast<int>(recipes->size()); ++i) {
                idxMap.push_back(i);
            }
        }
    }

    void schema() override {
        if (idxMap.empty()) {
            out.coutln("No recipes found.");
            return;
        }

        const int perPage = 10;
        int page = 0;
        int total = static_cast<int>(idxMap.size());
        int pages = max(1, (total + perPage - 1) / perPage);

        while (true) {
            out.br();
            out.coutln("Recipes (" + to_string(perPage) + " recipes/page)");
            int start = page * perPage;
            int end = min(total, start + perPage);
            for (int i = start; i < end; ++i) {
                int display = i - start + 1;
                out.coutln(to_string(display) + ". " + (*recipes)[idxMap[i]].name);
            }
            out.coutln("Page " + to_string(page+1) + "/" + to_string(pages));
            out.br();
            out.coutln("Options:");
            out.displayChoices({"View Recipe", "Next Page", "Previous Page", "Back"});
            int c = out.inputi("Enter your choice: ");

            if (c == 1) {
                int which = out.inputi("Which recipe?: ");
                if (which >= 1 && which <= (end-start)) {
                    const Recipe& r = (*recipes)[idxMap[start + (which-1)]];
                    out.br();
                    printRecipe(r);
                    out.br();
                    out.coutln("Options:");
                    out.displayChoices({"View Recipe", "Back to Recipes", "Back to Recipe Manager"});
                    int vr = out.inputi("Enter your choice: ");
                    if (vr == 3) return;
                }
            } else if (c == 2) {
                if (page + 1 < pages) page++;
            } else if (c == 3) {
                if (page > 0) page--;
            } else {
                return;
            }
        }
    }
};

#endif // VIEW_RECIPES_PAGE_H
