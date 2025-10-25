#include "../../vendor/sys/out.h"
#include "../../vendor/base/page.h"
#include "grocery.cpp"

using namespace std;

/*
    ViewGroceryPage Class

    This page displays all items currently in the grocery list.
    It loads all items from the CSV file and presents them in a formatted list.

    How it works:
        - Loads all grocery items from grocery.csv
        - Displays each item with its id, name, quantity, and unit
        - If list is empty, shows a message
        - Provides a back option to return to grocery manager

    Header classes:
    #include "../../vendor/sys/out.h"
    #include "../../vendor/base/page.h"
    #include "grocery.cpp"

    ViewGroceryPage:
        protected:
            - schema()                  : Display all grocery items (override from Page)
        public:
            - ViewGroceryPage()         : Constructor
*/
class ViewGroceryPage : public Page {
protected:
    /*
        Display all grocery items
    */
    void schema() override {
        out.hr();                                   // horizontal rule
        out.br();
        vector<GroceryItem> items = GroceryItem::loadAll();     // load all items
        if (items.size() == 0) {                    // list is empty
            out.coutln("Grocery list is empty.");   // display empty message
            out.br();
            return;                                 // exit schema
        }
        out.coutln("Grocery List (" + to_string(items.size()) + " item(s)):");  // display count
        out.br();
        for (int i = 0; i < items.size(); i++) {
            GroceryItem it = items[i];              // get current item
            out.coutln(to_string(i + 1) + ". " + it.name + " - " + it.quantity + " " + it.unit + " (ID: " + to_string(it.id) + ")");  // display item
        }
        out.br();
    }
public:
    /*
        Constructor
            - Set header and back behavior
    */
    ViewGroceryPage() : Page() {
        this->header = "View Grocery List";         // set page header
        this->showheader = true;                    // enable header display
        this->useExitInsteadOfBack = false;         // use back instead of exit
        this->options = {};                         // no additional options
    }
};
