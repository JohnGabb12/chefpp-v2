#include "../../vendor/sys/out.h"
#include "../../vendor/base/modal.h"
#include "mealplan.cpp"
#include "../recipemanager/recipe.cpp"

using namespace std;

/*
    GenerateMealPlanModal Class

    This modal generates a weekly meal plan.
    It can auto-fill the 7 days using available recipes or let the user pick per day.

    How it works:
        - Ask for week label (any string)
        - Optionally clear existing plan for that week
        - Ask to auto-fill or manual pick
        - Save 7 entries (Mon..Sun) to mealplan.csv

    Header classes:
    #include "../../vendor/sys/out.h"
    #include "../../vendor/base/modal.h"
    #include "mealplan.cpp"
    #include "../recipemanager/recipe.cpp"

    GenerateMealPlanModal:
        protected:
            - schema()      : Main logic to build a week plan
        public:
            - GenerateMealPlanModal() : Constructor
*/
class GenerateMealPlanModal : public Modal {
protected:
    void schema() override {
        out.subheader("Generate Meal Plan");
        string week = out.inputs("Enter week label (e.g., 2025-W43): ");
        if (week == "") { week = "Week"; }
        bool wipe = out.inputYesNo("Clear existing plan for this week? (y/n): ");
        if (wipe) { MealPlan::clearWeek(week); }
        out.br();

        vector<Recipe> recipes = Recipe::loadAll();
        if (recipes.size() == 0) {
            out.coutln("No recipes available. Please add recipes first.");
            return;
        }

        bool autoFill = out.inputYesNo("Auto-fill 7 days using available recipes? (y/n): ");
        out.br();

        string days[7] = {"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};

        if (autoFill) {
            for (int i = 0; i < 7; i++) {
                Recipe r = recipes[i % recipes.size()];
                MealPlan mp(week, days[i], r.id, r.name);
                mp.save();
            }
            out.coutln("Meal plan generated for week: " + week);
            return;
        }

        out.coutln("Pick a recipe for each day:");
        out.br();
        for (int i = 0; i < 7; i++) {
            out.coutln(days[i] + ":");
            for (int j = 0; j < recipes.size(); j++) {
                Recipe rr = recipes[j];
                out.coutln(to_string(j + 1) + ". " + rr.name + " (ID: " + to_string(rr.id) + ")");
            }
            int pick = 0;
            while (true) {
                pick = out.inputi("Select recipe number: ");
                if (pick >= 1 && pick <= recipes.size()) { break; }
                out.coutln("Invalid choice. Try again.");
            }
            Recipe chosen = recipes[pick - 1];
            MealPlan mp2(week, days[i], chosen.id, chosen.name);
            mp2.save();
            out.br();
        }
        out.coutln("Meal plan saved for week: " + week);
    }

public:
    GenerateMealPlanModal() : Modal() {}
};
