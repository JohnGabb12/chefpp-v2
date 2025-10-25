#include "../../vendor/sys/out.h"
#include "../../vendor/base/modal.h"
#include "recipe.cpp"
#include "../pantrymanager/pantry.cpp"
#include "../grocerymanager/grocery.cpp"

/*
    ViewRecipeModal Class

    This modal displays a single recipe's details.
    It shows the recipe name, ingredients list, and cooking instructions.

    How it works:
        - Takes a Recipe object in the constructor
        - Displays the recipe preview in a formatted box
        - Automatically returns to parent page after user presses Enter

    Header classes:
    #include "../../vendor/sys/out.h"
    #include "../../vendor/base/modal.h"
    #include "recipe.cpp"

    ViewRecipeModal:
        private:
            - recipe                : Recipe object to display
        protected:
            - schema()              : Main modal logic (override from Modal)
        public:
            - ViewRecipeModal()     : Constructor that takes a Recipe object
*/
class ViewRecipeModal : public Modal {
private:
    Recipe recipe;                                                          // recipe to display

    /*
        Parse ingredient token into name, amount, and unit
            - Supports "name|amount|unit" or "name:amount:unit"
            - Returns parsed values by reference
    */
    void parseIngredient(string token, string& name, string& amount, string& unit) {
        int sep1 = -1; int sep2 = -1; char separator = ' ';
        for (int i = 0; i < token.length(); i++) {              // find separator type
            if (token[i] == '|') { sep1 = i; separator = '|'; break; }
            if (token[i] == ':') { sep1 = i; separator = ':'; break; }
        }
        if (sep1 != -1) {                                       // first separator found
            for (int i = sep1 + 1; i < token.length(); i++) {   // find second separator
                if (token[i] == separator) { sep2 = i; break; }
            }
        }
        if (sep1 != -1 && sep2 != -1) {                         // both separators found
            name = out.trim(token.substr(0, sep1));             // extract name
            amount = out.trim(token.substr(sep1 + 1, sep2 - sep1 - 1));    // extract amount
            unit = out.trim(token.substr(sep2 + 1));            // extract unit
        } else {
            name = out.trim(token);                             // only name available
            amount = ""; unit = "";
        }
    }

    /*
        Find pantry item by name
            - Returns quantity and unit if found
            - Returns found status
    */
    bool findInPantry(string itemName, double& quantity, string& unit) {
        vector<Pantry> pantry = Pantry::loadAll();              // load all pantry items
        string nameLower = out.toLowerCase(itemName);           // normalize search name
        for (int i = 0; i < pantry.size(); i++) {               // search pantry
            if (out.toLowerCase(pantry[i].name) == nameLower) { // name match
                unit = pantry[i].unit;                          // get unit
                try { quantity = stod(pantry[i].quantity); } catch (...) { quantity = 0.0; }    // parse quantity
                return true;                                    // found
            }
        }
        return false;                                           // not found
    }

    /*
        Add item to grocery list with given quantity
    */
    void addToGrocery(string name, string quantity, string unit) {
        GroceryItem item(name, quantity, unit);                 // create grocery item
        item.save();                                            // save to list
    }

    /*
        Generate grocery list entries for this recipe
            - Compares required ingredients to pantry
            - Adds missing or insufficient amounts to grocery.csv
    */
    void generateGroceryForRecipe() {
        for (int i = 0; i < recipe.ingredients.size(); i++) {   // process each ingredient
            string name = ""; string amountStr = ""; string unit = "";
            parseIngredient(recipe.ingredients[i], name, amountStr, unit);  // parse ingredient

            double need = 0.0;
            if (amountStr != "") {                              // amount provided
                try { need = stod(amountStr); } catch (...) { need = 0.0; }     // parse amount
            }

            double have = 0.0; string pantryUnit = "";
            bool inPantry = findInPantry(name, have, pantryUnit);   // check pantry

            if (!inPantry) {                                    // not in pantry
                string qty = amountStr == "" ? "1" : amountStr; // default to 1 if unknown
                addToGrocery(name, qty, unit);                  // add full amount
            } else if (unit != "" && pantryUnit != "" && unit != pantryUnit) {  // unit mismatch
                string qty = amountStr == "" ? "1" : amountStr; // default to 1 if unknown
                addToGrocery(name, qty, unit);                  // add full amount (no conversion)
            } else if (need == 0.0 && have <= 0.0) {            // unknown amount and pantry empty
                addToGrocery(name, "1", unit);                  // add default 1
            } else if (need > have) {                           // need more than available
                double diff = need - have;                      // calculate shortage
                addToGrocery(name, to_string(diff), unit);      // add difference
            }
        }
        out.coutln("Grocery list generated based on pantry availability.");
        out.br();
    }

protected:
    /*
        Main modal schema - Display recipe details
    */
    void schema() override {
        recipe.displayPreview();                                            // show recipe in formatted box
        bool gen = out.inputYesNo("Generate grocery list for missing ingredients? (y/n): ");  // prompt for grocery generation
        if (gen) { generateGroceryForRecipe(); }    // generate grocery list if confirmed
    }

public:
    /*
        Constructor - Initialize with recipe to display
    */
    ViewRecipeModal(Recipe r) {
        recipe = r;                                                         // store recipe to display
    }
};
