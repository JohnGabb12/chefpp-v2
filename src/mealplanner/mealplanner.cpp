#include "../../vendor/sys/out.h"
#include "../../vendor/base/page.h"
#include "generatemealplan.cpp"
#include "viewmealplan.cpp"

using namespace std;

/*
    MealPlannerPage Class

    Entry page for Meal Planner.
    Provides options to generate and view weekly plans.

    How it works:
        - Shows two options: Generate and View
        - Navigates to the corresponding modal/page

    Header classes:
    #include "../../vendor/sys/out.h"
    #include "../../vendor/base/page.h"
    #include "generatemealplan.cpp"
    #include "viewmealplan.cpp"

    MealPlannerPage:
        protected:
            - schema()              : Page content display (override from Page)
        public:
            - MealPlannerPage()     : Constructor and options wiring
*/
class MealPlannerPage : public Page {
protected:
    /*
        Page schema - no content needed, just options
    */
    void schema() override {
        // no additional content to display
    }

public:
    MealPlannerPage() : Page() {
        this->header = "Meal Planner";
        this->showheader = true;
        this->useExitInsteadOfBack = false;
        this->options = {
            Options("Generate", [this]() {
                GenerateMealPlanModal modal; modal.init();
            }),
            Options("View", [this]() {
                ViewMealPlanPage page; page.init();
            })
        };
    }
};
