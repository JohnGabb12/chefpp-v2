#include "../../vendor/sys/out.h"
#include "../../vendor/base/modal.h"
#include "recipe.cpp"

/*
    ViewRecipeModal Class

    This modal displays a single recipe's details.
    It shows the recipe name, ingredients list, and cooking instructions.

    How it works:
        - Takes a Recipe object in the constructor
        - Displays the recipe preview in a formatted box
        - Automatically returns to parent page after user presses Enter

    Header classes:
    #include "../../vendor/sys/out.h"
    #include "../../vendor/base/modal.h"
    #include "recipe.cpp"

    ViewRecipeModal:
        private:
            - recipe                : Recipe object to display
        protected:
            - schema()              : Main modal logic (override from Modal)
        public:
            - ViewRecipeModal()     : Constructor that takes a Recipe object
*/
class ViewRecipeModal : public Modal {
private:
    Recipe recipe;                                                          // recipe to display

protected:
    /*
        Main modal schema - Display recipe details
    */
    void schema() override {
        recipe.displayPreview();                                            // show recipe in formatted box
    }

public:
    /*
        Constructor - Initialize with recipe to display
    */
    ViewRecipeModal(Recipe r) {
        recipe = r;                                                         // store recipe to display
    }
};
