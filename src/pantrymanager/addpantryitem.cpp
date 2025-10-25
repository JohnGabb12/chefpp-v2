#include "../../vendor/sys/out.h"
#include "../../vendor/base/modal.h"
#include "pantry.cpp"

using namespace std;

/*
    AddPantryItemModal Class

    This modal handles adding new ingredients to the pantry inventory.
    It collects ingredient data, displays a preview, and allows editing before saving.

    How it works:
        - Collects ingredient name, quantity, and unit from user
        - Displays a preview of the ingredient
        - Asks for confirmation (y/n)
        - If not confirmed, allows editing specific fields
        - Loops until confirmed, then saves to CSV file
        - Automatically returns to parent page after completion

    Header classes:
    #include "../../vendor/sys/out.h"
    #include "../../vendor/base/modal.h"
    #include "pantry.cpp"

    AddPantryItemModal:
        private:
            - item                      : Pantry object being created
            - collectItemData()         : Prompts user for ingredient information
            - confirmItem()             : Shows preview and asks for confirmation
            - changeItem()              : Allows editing specific ingredient fields
        protected:
            - schema()                  : Main modal logic (override from Modal)
        public:
            - AddPantryItemModal()      : Constructor
*/
class AddPantryItemModal : public Modal {
private:
    Pantry item;                                    // pantry item being created

    /*
        Collect ingredient data from user input
    */
    void collectItemData() {
        out.hr();                                   // horizontal rule
        out.br();
        item.name = out.inputs("Enter ingredient name: ");      // prompt for name
        item.quantity = out.inputs("Enter quantity: ");         // prompt for quantity
        item.unit = out.inputs("Enter unit (cups, g, tsp, etc): ");    // prompt for unit
        out.br();
    }

    /*
        Display ingredient preview and ask for confirmation
    */
    bool confirmItem() {
        out.clear();                                // clear screen
        item.displayPreview();                      // show ingredient details
        return out.inputYesNo("Are you sure? (y/n): ");     // prompt for confirmation
    }

    /*
        Allow user to edit specific ingredient fields
    */
    void changeItem() {
        out.br();
        out.coutln("What do you want to change:");  // prompt for edit choice
        out.coutln("1. Ingredient name");
        out.coutln("2. Quantity");
        out.coutln("3. Unit");
        out.coutln("4. None");
        int choice = out.inputi("Enter your choice: ");     // get user choice
        out.br();

        switch (choice) {
            case 1:
                item.name = out.inputs("Ingredient name: ");    // edit name
                out.coutln("Ingredient name changed successfully!");
                out.br();
                break;
            case 2:
                item.quantity = out.inputs("Quantity: ");   // edit quantity
                out.coutln("Quantity changed successfully!");
                out.br();
                break;
            case 3:
                item.unit = out.inputs("Unit: ");           // edit unit
                out.coutln("Unit changed successfully!");
                out.br();
                break;
            case 4:
                return;                             // no changes
            default:
                out.coutln("Invalid choice!");      // invalid input
                break;
        }
    }

protected:
    /*
        Main modal schema - Add pantry item workflow
    */
    void schema() override {
        collectItemData();                          // collect ingredient data
        
        while (true) {
            if (confirmItem()) {                    // user confirmed
                item.save();                        // save to csv
                item = Pantry();                    // reset item
                return;                             // exit modal
            } else {
                changeItem();                       // edit fields
            }
        }
    }
};
