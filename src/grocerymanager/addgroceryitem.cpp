#include "../../vendor/sys/out.h"
#include "../../vendor/base/modal.h"
#include "grocery.cpp"

using namespace std;

/*
    AddGroceryItemModal Class

    This modal handles adding new items to the grocery list.
    It collects item data, displays a preview, and allows editing before saving.

    How it works:
        - Collects item name, quantity, and unit from user
        - Displays a preview of the item
        - Asks for confirmation (y/n)
        - If not confirmed, allows editing specific fields
        - Loops until confirmed, then saves to CSV file
        - Automatically returns to parent page after completion
        - If duplicate (same name+unit), adds quantities instead of creating new entry

    Header classes:
    #include "../../vendor/sys/out.h"
    #include "../../vendor/base/modal.h"
    #include "grocery.cpp"

    AddGroceryItemModal:
        private:
            - item                      : GroceryItem object being created
            - collect()                 : Prompts user for item information
            - confirm()                 : Shows preview and asks for confirmation
            - edit()                    : Allows editing specific item fields
        protected:
            - schema()                  : Main modal logic (override from Modal)
        public:
            - AddGroceryItemModal()     : Constructor
*/
class AddGroceryItemModal : public Modal {
private:
    GroceryItem item;                               // grocery item being created

    /*
        Collect item data from user input
    */
    void collect() {
        out.hr();                                   // horizontal rule
        out.br();
        item.name = out.inputs("Enter item name: ");    // prompt for name
        item.quantity = out.inputs("Enter quantity: "); // prompt for quantity
        item.unit = out.inputs("Enter unit (optional): ");  // prompt for unit
        out.br();
    }

    /*
        Display item preview and ask for confirmation
    */
    bool confirm() {
        out.clear();                                // clear screen
        out.coutln("+-----------------------------------------+");
        out.coutln("Item: " + item.name);           // display item name
        out.coutln("Quantity: " + item.quantity + " " + item.unit);    // display quantity and unit
        out.coutln("+-----------------------------------------+");
        out.br();
        return out.inputYesNo("Add to grocery list? (y/n): ");     // prompt for confirmation
    }

    /*
        Allow user to edit specific item fields
    */
    void edit() {
        out.br();
        out.coutln("What do you want to change:");  // prompt for edit choice
        out.coutln("1. Name");
        out.coutln("2. Quantity");
        out.coutln("3. Unit");
        out.coutln("4. None");
        int c = out.inputi("Enter your choice: ");  // get user choice
        out.br();
        if (c == 1) { item.name = out.inputs("Name: "); }      // edit name
        else if (c == 2) { item.quantity = out.inputs("Quantity: "); }     // edit quantity
        else if (c == 3) { item.unit = out.inputs("Unit: "); }             // edit unit
    }

protected:
    /*
        Main modal schema - Add grocery item workflow
    */
    void schema() override {
        collect();                                  // collect item data
        while (true) {
            if (confirm()) {                        // user confirmed
                item.save();                        // save to csv (merge if duplicate)
                return;                             // exit modal
            } else {
                edit();                             // edit fields
            }
        }
    }
};
