#ifndef CHECK_INGREDIENTS_PAGE_H
#define CHECK_INGREDIENTS_PAGE_H

#include "../../vendor/page.h"
#include "../../vendor/sysfunc/out.h"
#include "Recipe.h"
#include <vector>
#include <algorithm>

using namespace std;

class CheckIngredientsPage : public Page {
private:
    vector<Recipe>* recipes;

public:
    CheckIngredientsPage(vector<Recipe>* recipesRef)
        : Page("Check Ingredients", "Check Ingredients", {"Recipe Manager"}), recipes(recipesRef) {}

    void schema() override {
        if (recipes->empty()) {
            out.coutln("No recipes to check.");
            return;
        }

        for (int i = 0; i < static_cast<int>(recipes->size()); ++i) {
            out.coutln(to_string(i+1) + ". " + (*recipes)[i].name);
        }

        int idx = out.inputi("Which recipe?: ") - 1;
        if (idx < 0 || idx >= static_cast<int>(recipes->size())) {
            out.coutln("Invalid selection.");
            return;
        }

        string haveStr = out.inputs("Enter available ingredients (comma-separated): ");
        vector<string> have = RecipeUtils::splitCsv(haveStr);

        // Make lowercase sets
        vector<string> haveLow = have;
        for (auto& s : haveLow) {
            s = RecipeUtils::toLower(s);
        }

        vector<string> missing;
        for (const auto& ing : (*recipes)[idx].ingredients) {
            string low = RecipeUtils::toLower(ing);
            if (find(haveLow.begin(), haveLow.end(), low) == haveLow.end()) {
                missing.push_back(ing);
            }
        }

        out.br();
        if (missing.empty()) {
            out.coutln("You have all the ingredients!");
        } else {
            out.coutln("Missing ingredients:");
            for (const auto& m : missing) {
                out.coutln("- " + m);
            }
        }
    }
};

#endif // CHECK_INGREDIENTS_PAGE_H
