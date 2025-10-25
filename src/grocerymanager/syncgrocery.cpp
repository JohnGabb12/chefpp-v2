#include "../../vendor/sys/out.h"
#include "../../vendor/base/modal.h"
#include "grocery.cpp"
#include "../pantrymanager/pantry.cpp"

using namespace std;

/*
    SyncGroceryWithPantryModal Class

    This modal handles syncing grocery list items into the pantry.
    It moves all items from the grocery list to the pantry and clears the grocery list.

    How it works:
        - Loads all grocery items
        - Displays what will be synced to pantry
        - Asks for confirmation (y/n)
        - If confirmed, adds each item to pantry (duplicates are merged)
        - Clears the entire grocery list after sync
        - Automatically returns to parent page after completion

    Header classes:
    #include "../../vendor/sys/out.h"
    #include "../../vendor/base/modal.h"
    #include "grocery.cpp"
    #include "../pantrymanager/pantry.cpp"

    SyncGroceryWithPantryModal:
        protected:
            - schema()                  : Main modal logic (override from Modal)
        public:
            - SyncGroceryWithPantryModal() : Constructor
*/
class SyncGroceryWithPantryModal : public Modal {
protected:
    /*
        Main modal schema - Sync grocery list to pantry workflow
    */
    void schema() override {
        out.hr();                                   // horizontal rule
        out.br();
        vector<GroceryItem> items = GroceryItem::loadAll();     // load all items
        if (items.size() == 0) {                    // list is empty
            out.coutln("Nothing to sync. Grocery list is empty.");
            out.br();
            return;                                 // exit modal
        }
        out.coutln("Sync will add these items to your pantry:");    // display header
        out.br();
        for (int i = 0; i < items.size(); i++) {
            GroceryItem it = items[i];              // get current item
            out.coutln("- " + it.name + " (" + it.quantity + " " + it.unit + ")");     // display item
        }
        out.br();
        bool ok = out.inputYesNo("Proceed to sync and clear grocery list? (y/n): ");   // prompt for confirmation
        if (!ok) { out.coutln("Sync cancelled."); out.br(); return; }   // user cancelled
        
        for (int i = 0; i < items.size(); i++) {
            GroceryItem it = items[i];              // get current item
            Pantry p(it.name, it.quantity, it.unit);    // create pantry item
            p.save();                               // save to pantry (merge if duplicate)
        }
        GroceryItem::clearAll();                    // clear grocery list
        out.coutln("Sync complete. Grocery list cleared.");
        out.br();
    }
};
