#include "../../vendor/sys/out.h"
#include "../../vendor/base/modal.h"
#include "pantry.cpp"

using namespace std;

/*
    DeletePantryItemModal Class

    This modal handles deleting ingredients from the pantry inventory.
    It prompts for an ingredient ID, displays details, and confirms before deleting.

    How it works:
        - Prompts user for ingredient ID
        - Loads the ingredient and displays details
        - Asks for confirmation (y/n)
        - Deletes from CSV file if confirmed
        - Automatically returns to parent page after completion

    Header classes:
    #include "../../vendor/sys/out.h"
    #include "../../vendor/base/modal.h"
    #include "pantry.cpp"

    DeletePantryItemModal:
        protected:
            - schema()                  : Main modal logic (override from Modal)
        public:
            - DeletePantryItemModal()   : Constructor
*/
class DeletePantryItemModal : public Modal {
protected:
    /*
        Main modal schema - Delete pantry item workflow
    */
    void schema() override {
        out.hr();                                   // horizontal rule
        out.br();
        
        vector<Pantry> items = Pantry::loadAll();   // load all pantry items
        
        if (items.size() == 0) {                    // pantry is empty
            out.coutln("Your pantry is empty. Nothing to delete.");
            out.br();
            return;                                 // exit modal
        }
        
        out.coutln("Current Pantry Items:");        // display header
        out.br();
        
        for (int i = 0; i < items.size(); i++) {
            Pantry item = items[i];                 // get current item
            string line = to_string(i + 1) + ". " + item.name + " - " + item.quantity + " " + item.unit + " (ID: " + to_string(item.id) + ")";    // format item line
            out.coutln(line);                       // display item
        }
        
        out.br();
        int id = out.inputi("Enter ingredient ID to delete: ");    // prompt for id
        
        Pantry item = Pantry::findById(id);         // find ingredient by id
        
        if (item.id == 0) {                         // ingredient not found
            out.br();
            out.coutln("Ingredient not found!");    // display error
            out.br();
            return;                                 // exit modal
        }
        
        out.br();
        out.coutln("Ingredient to delete:");        // display confirmation header
        item.displayPreview();                      // show ingredient details
        
        bool confirm = out.inputYesNo("Are you sure you want to delete this ingredient? (y/n): ");   // prompt for confirmation
        
        if (confirm) {                              // user confirmed
            if (Pantry::deleteById(id)) {           // delete ingredient
                out.br();
                out.coutln("Ingredient deleted successfully!");
                out.br();
            } else {
                out.br();
                out.coutln("Error: Could not delete ingredient.");  // display error
                out.br();
            }
        } else {
            out.br();
            out.coutln("Deletion cancelled.");      // user cancelled
            out.br();
        }
    }

public:
    /*
        Constructor
    */
    DeletePantryItemModal() : Modal() {}            // default constructor
};
