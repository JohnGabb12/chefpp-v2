#include "../../vendor/sys/out.h"
#include "../../vendor/base/page.h"
#include "addgroceryitem.cpp"
#include "removegroceryitem.cpp"
#include "viewgrocery.cpp"
#include "syncgrocery.cpp"
#include "generategrocery.cpp"

using namespace std;

/*
    GroceryManagerPage Class

    This page serves as the entry point for all grocery list features.
    It wires up the grocery management menu and navigates to specific
    flows such as generating from recipe, adding, removing, viewing, and syncing items.

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
    #include "addgroceryitem.cpp"           // AddGroceryItemModal (modal)
    #include "removegroceryitem.cpp"        // RemoveGroceryItemModal (modal)
    #include "viewgrocery.cpp"              // ViewGroceryPage (page)
    #include "syncgrocery.cpp"              // SyncGroceryWithPantryModal (modal)
    #include "generategrocery.cpp"          // GenerateGroceryFromRecipeModal (modal)

    Options:
        - "Generate from Recipe"  : Opens GenerateGroceryFromRecipeModal (select recipe → compare pantry → add missing → return)
        - "Add Item"              : Opens AddGroceryItemModal (collect → confirm → save → return)
        - "Remove Item"           : Opens RemoveGroceryItemModal (list → select → confirm → delete → return)
        - "View Grocery List"     : Opens ViewGroceryPage (displays all items)
        - "Sync with Pantry"      : Opens SyncGroceryWithPantryModal (list → confirm → add to pantry → clear list → return)
*/
class GroceryManagerPage : public Page {
public:
    /*
        Constructor
            - Set header visibility and back behavior
            - Register options with their corresponding actions
    */
    GroceryManagerPage() : Page() {
        this->header = "Grocery List Manager";      // set page header
        this->showheader = true;                    // enable header display
        this->useExitInsteadOfBack = false;         // use back instead of exit

        this->options = {
            Options("Generate from Recipe", []() {  // option 1: generate from recipe
                GenerateGroceryFromRecipeModal gen; // create modal instance
                gen.init();                         // open modal
            }),
            Options("Add Item", []() {              // option 2: add item
                AddGroceryItemModal add;            // create modal instance
                add.init();                         // open modal
            }),
            Options("Remove Item", []() {           // option 3: remove item
                RemoveGroceryItemModal rem;         // create modal instance
                rem.init();                         // open modal
            }),
            Options("View Grocery List", []() {     // option 4: view list
                ViewGroceryPage view;               // create page instance
                view.init();                        // open page
            }),
            Options("Sync with Pantry", []() {      // option 5: sync to pantry
                SyncGroceryWithPantryModal sync;    // create modal instance
                sync.init();                        // open modal
            })
        };
    }

protected:
    /*
        Schema
            - This page is purely a menu; no additional static content
              is required here. The base Page class handles rendering
              the header and the options list.
    */
    void schema() override {}                       // empty schema (menu only)
};
