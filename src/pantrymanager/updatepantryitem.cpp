#include "../../vendor/sys/out.h"
#include "../../vendor/base/modal.h"
#include "pantry.cpp"

using namespace std;

/*
    UpdatePantryItemModal Class

    This modal handles updating ingredient quantities in the pantry inventory.
    It prompts for an ingredient ID, displays current quantity, and allows updating.

    How it works:
        - Prompts user for ingredient ID
        - Loads the ingredient and displays current details
        - Asks for new quantity
        - Updates the CSV file with the new quantity
        - Automatically returns to parent page after completion

    Header classes:
    #include "../../vendor/sys/out.h"
    #include "../../vendor/base/modal.h"
    #include "pantry.cpp"

    UpdatePantryItemModal:
        protected:
            - schema()                  : Main modal logic (override from Modal)
        public:
            - UpdatePantryItemModal()   : Constructor
*/
class UpdatePantryItemModal : public Modal {
protected:
    /*
        Main modal schema - Update pantry item workflow
    */
    void schema() override {
        out.hr();                                   // horizontal rule
        out.br();
        
        vector<Pantry> items = Pantry::loadAll();   // load all pantry items
        
        if (items.size() == 0) {                    // pantry is empty
            out.coutln("Your pantry is empty. Nothing to update.");
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
        int id = out.inputi("Enter ingredient ID to update: ");    // prompt for id
        
        Pantry item = Pantry::findById(id);         // find ingredient by id
        
        if (item.id == 0) {                         // ingredient not found
            out.br();
            out.coutln("Ingredient not found!");    // display error
            out.br();
            return;                                 // exit modal
        }
        
        out.br();
        out.coutln("Current details:");             // display current details
        item.displayPreview();                      // show ingredient details
        
        string newQuantity = out.inputs("Enter new quantity: ");   // prompt for new quantity
        
        if (item.updateQuantity(newQuantity)) {     // update quantity
            out.br();
            out.coutln("Quantity updated successfully!");
            out.br();
            out.coutln("Updated details:");         // display updated details
            item.displayPreview();                  // show updated ingredient
        } else {
            out.br();
            out.coutln("Error: Could not update quantity.");    // display error
            out.br();
        }
    }

public:
    /*
        Constructor
    */
    UpdatePantryItemModal() : Modal() {}            // default constructor
};
