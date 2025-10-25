#include "../../vendor/sys/out.h"
#include "../../vendor/base/modal.h"
#include "grocery.cpp"

using namespace std;

/*
    RemoveGroceryItemModal Class

    This modal handles deleting items from the grocery list.
    It prompts for an item ID, displays details, and confirms before deleting.

    How it works:
        - Loads all grocery items and displays them
        - Prompts user for item ID to remove
        - Displays the item and asks for confirmation (y/n)
        - Deletes from CSV file if confirmed
        - Automatically returns to parent page after completion

    Header classes:
    #include "../../vendor/sys/out.h"
    #include "../../vendor/base/modal.h"
    #include "grocery.cpp"

    RemoveGroceryItemModal:
        protected:
            - schema()                  : Main modal logic (override from Modal)
        public:
            - RemoveGroceryItemModal()  : Constructor
*/
class RemoveGroceryItemModal : public Modal {
protected:
    /*
        Main modal schema - Remove grocery item workflow
    */
    void schema() override {
        out.hr();                                   // horizontal rule
        out.br();
        vector<GroceryItem> items = GroceryItem::loadAll();     // load all items
        if (items.size() == 0) {                    // list is empty
            out.coutln("Grocery list is empty.");   // display empty message
            out.br();
            return;                                 // exit modal
        }
        out.coutln("Current Grocery List:");        // display header
        out.br();
        for (int i = 0; i < items.size(); i++) {
            GroceryItem it = items[i];              // get current item
            out.coutln(to_string(i + 1) + ". " + it.name + " - " + it.quantity + " " + it.unit + " (ID: " + to_string(it.id) + ")");  // display item
        }
        out.br();
        int id = out.inputi("Enter item ID to remove: ");   // prompt for id
        GroceryItem found = GroceryItem::findById(id);      // find item by id
        if (found.id == 0) {                        // item not found
            out.br();
            out.coutln("Item not found!");          // display error
            out.br();
            return;                                 // exit modal
        }
        out.br();
        bool ok = out.inputYesNo("Are you sure you want to delete this item? (y/n): ");    // prompt for confirmation
        if (!ok) { out.coutln("Deletion cancelled."); out.br(); return; }   // user cancelled
        if (GroceryItem::deleteById(id)) {          // delete item
            out.coutln("Item deleted successfully!");
            out.br();
        } else {
            out.coutln("Error: Could not delete item.");    // display error
            out.br();
        }
    }
};
