#include "../../vendor/sys/out.h"
#include "../../vendor/base/page.h"
#include "mealplan.cpp"

using namespace std;

/*
    ViewMealPlanPage Class

    Displays all planned meals for a given week.
    Lists the day and the selected recipe.

    How it works:
        - Ask for week label
        - Load entries for that week
        - Display in a simple list

    Header classes:
    #include "../../vendor/sys/out.h"
    #include "../../vendor/base/page.h"
    #include "mealplan.cpp"

    ViewMealPlanPage:
        protected:
            - schema()      : Render view for a selected week
        public:
            - ViewMealPlanPage() : Constructor
*/
class ViewMealPlanPage : public Page {
protected:
    void schema() override {
        out.hr();
        out.br();
        string week = out.inputs("Enter week label to view: ");
        if (week == "") { out.coutln("No week provided."); out.br(); return; }
        vector<MealPlan> items = MealPlan::findByWeek(week);
        if (items.size() == 0) {
            out.coutln("No meal plan found for week: " + week);
            out.br();
            return;
        }
        out.coutln("Meal Plan for " + week + " (" + to_string(items.size()) + " item(s)):");
        out.br();
        for (int i = 0; i < items.size(); i++) {
            MealPlan it = items[i];
            out.coutln("- " + it.day + ": " + it.recipeName + " (Recipe ID: " + to_string(it.recipeId) + ")");
        }
        out.br();
    }
public:
    ViewMealPlanPage() : Page() {
        this->header = "View Meal Plan";
        this->showheader = true;
        this->useExitInsteadOfBack = false;
        this->options = {};
    }
};
