#include "../../vendor/sys/out.h"
#include "../../vendor/base/page.h"
#include "pantry.cpp"

using namespace std;

/*
    ViewPantryPage Class

    This page displays all ingredients currently in the pantry inventory.
    It loads all items from the CSV file and presents them in a formatted list.

    How it works:
        - Loads all pantry items from pantry.csv
        - Displays each item with its id, name, quantity, and unit
        - If pantry is empty, shows a message
        - Provides a back option to return to pantry manager

    Header classes:
    #include "../../vendor/sys/out.h"
    #include "../../vendor/base/page.h"
    #include "pantry.cpp"

    ViewPantryPage:
        protected:
            - schema()                  : Display all pantry items (override from Page)
        public:
            - ViewPantryPage()          : Constructor
*/
class ViewPantryPage : public Page {
protected:
    /*
        Display all pantry items
    */
    void schema() override {
        out.hr();                                   // horizontal rule
        out.br();
        
        vector<Pantry> items = Pantry::loadAll();   // load all pantry items
        
        if (items.size() == 0) {                    // pantry is empty
            out.coutln("Your pantry is empty.");    // display empty message
            out.br();
            return;                                 // exit schema
        }
        
        out.coutln("Pantry Inventory (" + to_string(items.size()) + " item(s)):");  // display count
        out.br();
        
        for (int i = 0; i < items.size(); i++) {
            Pantry item = items[i];                 // get current item
            string line = to_string(i + 1) + ". " + item.name + " - " + item.quantity + " " + item.unit + " (ID: " + to_string(item.id) + ")";    // format item line
            out.coutln(line);                       // display item
        }
        
        out.br();
    }

public:
    /*
        Constructor
            - Set header and back behavior
    */
    ViewPantryPage() : Page() {
        this->header = "View Pantry";               // set page header
        this->showheader = true;                    // enable header display
        this->useExitInsteadOfBack = false;         // use back instead of exit
        this->options = {};                         // no additional options
    }
};
