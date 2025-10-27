
#include "../../vendor/sys/out.h"
#include "../../vendor/base/page.h"
#include "addrecipe.cpp"
#include "searchrecipe.cpp"
#include "deleterecipe.cpp"

/*
    RecipeManagerPage Class

    This page serves as the entry point for all recipe-related features.
    It wires up the recipe management menu and navigates to specific
    flows such as adding, searching, deleting, and checking ingredients.

    How it works:
        - Inherits from Page to use the standard page lifecycle:
          header display, options list, and input handling.
        - The constructor configures the page header and populates the
          options vector with labeled actions and their callbacks.
        - Selecting an option may open another Page (navigates and returns)
          or a Modal (performs its task and auto-returns).

    Header classes:
    #include "../../vendor/sys/out.h"
    #include "../../vendor/base/page.h"
    #include "addrecipe.cpp"          // AddRecipeModal (modal)
    #include "searchrecipe.cpp"       // SearchRecipePage (page)
    #include "deleterecipe.cpp"       // DeleteRecipeModal (modal)

    Options:
        - "Add Recipe"        : Opens AddRecipeModal (collect → confirm → save → return)
        - "Search Recipe"     : Opens SearchRecipePage (search by name/ingredient/id, view modal)
        - "Delete Recipe"     : Opens DeleteRecipeModal (prompt for id → confirm → delete → return)
        - "Check Ingredients" : Placeholder for future implementation
*/

// RecipeManagerPage derives from Page to reuse the base navigation framework.
class RecipeManagerPage : public Page {

public:
    /*
        Constructor
            - Set header visibility and back behavior
            - Register options with their corresponding actions
    */
    RecipeManagerPage() : Page() {
        this->header = "Recipe Manager";              // page header text
        this->showheader = true;                       // show the header
        this->useExitInsteadOfBack = false;            // show "Back" instead of "Exit"

        // Populate the options vector with labeled actions
        this->options = {
            // Open the Add Recipe flow (modal). After finishing, it auto-returns here.
            Options("Add Recipe", []() {
                AddRecipeModal addRecipeModal;        // instantiate modal by value
                addRecipeModal.init();                // run modal and return
            }),

            // Open the Search Recipe page (can view recipe in a modal, or research)
            Options("Search Recipe", []() {
                SearchRecipePage searchPage;          // instantiate page by value
                searchPage.init();                    // navigate into page
            }),

            // Open the Delete Recipe flow (modal). Prompts for ID, confirms, and deletes.
            Options("Delete Recipe", []() {
                DeleteRecipeModal deleteRecipeModal;  // instantiate modal by value
                deleteRecipeModal.init();             // run modal and return
            }),
        };
    }

    /*
        Schema
            - This page is purely a menu; no additional static content
              is required here. The base Page class handles rendering
              the header and the options list.
    */
    void schema() override {}

};
