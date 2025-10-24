
#include "../../vendor/sys/out.h"
#include "../../vendor/base/page.h"
#include "addrecipe.cpp"

class RecipeManagerPage : public Page {

public:
    RecipeManagerPage() : Page() {
        this->header = "Recipe Manager";
        this->showheader = true;
        this->useExitInsteadOfBack = false;
        this->options = {
            Options("Add Recipe", []() {
                AddRecipeModal addRecipeModal;
                addRecipeModal.init();
            }),
            Options("View Recipes", []() {
                out.coutln("View Recipes selected");
            }),
            Options("Delete Recipe", []() {
                out.coutln("Delete Recipe selected");
            })
        };
    }

    void schema() override {
        out.coutln(out.center("Manage your recipes here."));
    }

};
