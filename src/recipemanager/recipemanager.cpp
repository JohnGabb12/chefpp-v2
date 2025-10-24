
#include "../../vendor/sys/out.h"
#include "../../vendor/base/page.h"
#include "addrecipe.cpp"
#include "searchrecipe.cpp"

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
            Options("Search Recipe", []() {
                SearchRecipePage searchPage;
                searchPage.init();
            }),
            Options("Delete Recipe", []() {
                out.coutln("Delete Recipe selected");
            }),
            Options("Check Ingredients", []() {
                out.coutln("Check Ingredients selected");
            })
        };
    }

    void schema() override {}

};
