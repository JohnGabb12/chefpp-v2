#include "../../vendor/sys/out.h"
#include "../../vendor/base/page.h"
#include "addpantryitem.cpp"
#include "viewpantry.cpp"
#include "updatepantryitem.cpp"
#include "deletepantryitem.cpp"

using namespace std;

/*
    PantryManagerPage Class

    This page serves as the entry point for all pantry inventory features.
    It wires up the pantry management menu and navigates to specific
    flows such as adding, viewing, updating, and deleting ingredients.

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
    #include "addpantryitem.cpp"        // AddPantryItemModal (modal)
    #include "viewpantry.cpp"           // ViewPantryPage (page)
    #include "updatepantryitem.cpp"     // UpdatePantryItemModal (modal)
    #include "deletepantryitem.cpp"     // DeletePantryItemModal (modal)

    Options:
        - "Add Ingredient"     : Opens AddPantryItemModal (collect → confirm → save → return)
        - "View Pantry"        : Opens ViewPantryPage (displays all pantry items)
        - "Update Quantity"    : Opens UpdatePantryItemModal (prompt for id → update → return)
        - "Delete Ingredient"  : Opens DeletePantryItemModal (prompt for id → confirm → delete → return)
*/
class PantryManagerPage : public Page {
public:
    /*
        Constructor
            - Set header visibility and back behavior
            - Register options with their corresponding actions
    */
    PantryManagerPage() : Page() {
        this->header = "Pantry Inventory Manager";  // set page header
        this->showheader = true;                    // enable header display
        this->useExitInsteadOfBack = false;         // use back instead of exit

        this->options = {
            Options("Add Ingredient", []() {        // option 1: add ingredient
                AddPantryItemModal addModal;        // create modal instance
                addModal.init();                    // open modal
            }),

            Options("View Pantry", []() {           // option 2: view pantry
                ViewPantryPage viewPage;            // create page instance
                viewPage.init();                    // open page
            }),

            Options("Update Quantity", []() {       // option 3: update quantity
                UpdatePantryItemModal updateModal;  // create modal instance
                updateModal.init();                 // open modal
            }),

            Options("Delete Ingredient", []() {     // option 4: delete ingredient
                DeletePantryItemModal deleteModal;  // create modal instance
                deleteModal.init();                 // open modal
            })
        };
    }

    /*
        Schema
            - This page is purely a menu; no additional static content
              is required here. The base Page class handles rendering
              the header and the options list.
    */
    void schema() override {}                       // empty schema (menu only)
};
