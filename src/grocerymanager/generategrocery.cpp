#include "../../vendor/sys/out.h"
#include "../../vendor/base/modal.h"
#include "grocery.cpp"
#include "../recipemanager/recipe.cpp"
#include "../pantrymanager/pantry.cpp"
#include <vector>

using namespace std;

/*
    GenerateGroceryFromRecipeModal Class

    This modal generates grocery list items from a recipe by comparing
    required ingredients against what's available in the pantry.

    How it works:
        - Lists all available recipes
        - User selects a recipe by number
        - For each ingredient in the recipe:
            * Parses ingredient format (name|amount|unit or name:amount:unit)
            * Checks pantry for matching ingredient (case-insensitive name)
            * If pantry has enough, nothing is added
            * If pantry has less, adds the difference to grocery list
            * If pantry doesn't have it, adds full required amount
            * If unit mismatch, adds full required amount (no unit conversion)
        - Automatically returns to parent page after completion

    Header classes:
    #include "../../vendor/sys/out.h"
    #include "../../vendor/base/modal.h"
    #include "grocery.cpp"
    #include "../recipemanager/recipe.cpp"
    #include "../pantrymanager/pantry.cpp"

    GenerateGroceryFromRecipeModal:
        private:
            - recipes                   : Vector of all recipes
            - listRecipes()             : Display all available recipes
            - addMissingForRecipe()     : Generate grocery items for a recipe
        protected:
            - schema()                  : Main modal logic (override from Modal)
        public:
            - GenerateGroceryFromRecipeModal() : Constructor
*/
class GenerateGroceryFromRecipeModal : public Modal {
private:
    vector<Recipe> recipes;                         // store all recipes

    /*
        List all available recipes
    */
    void listRecipes() {
        recipes = Recipe::loadAll();                // load all recipes
        if (recipes.size() == 0) {                  // no recipes available
            out.coutln("No recipes available.");    // display message
            out.br();
            return;                                 // exit method
        }
        out.coutln("Recipes:");                     // display header
        out.br();
        for (int i = 0; i < recipes.size(); i++) {
            out.coutln(to_string(i + 1) + ". " + recipes[i].name + " (ID: " + to_string(recipes[i].id) + ")");  // display recipe
        }
        out.br();
    }

    /*
        Add missing ingredients for a recipe to grocery list
    */
    void addMissingForRecipe(Recipe r) {
        for (int i = 0; i < r.ingredients.size(); i++) {    // loop through ingredients
            string token = r.ingredients[i];        // get ingredient token
            string iname = ""; string iamount = ""; string iunit = "";
            int p1 = -1; int p2 = -1;
            for (int j = 0; j < token.length(); j++) { if (token[j] == '|') { p1 = j; break; } }    // find first '|'
            if (p1 != -1) { for (int j = p1 + 1; j < token.length(); j++) { if (token[j] == '|') { p2 = j; break; } } }  // find second '|'
            if (p1 != -1 && p2 != -1) {             // pipe separator found
                iname = out.trim(token.substr(0, p1));      // extract name
                iamount = out.trim(token.substr(p1 + 1, p2 - p1 - 1));      // extract amount
                iunit = out.trim(token.substr(p2 + 1));     // extract unit
            } else {
                int c1 = -1; int c2 = -1;
                for (int j = 0; j < token.length(); j++) { if (token[j] == ':') { c1 = j; break; } }    // find first ':'
                if (c1 != -1) { for (int j = c1 + 1; j < token.length(); j++) { if (token[j] == ':') { c2 = j; break; } } }  // find second ':'
                if (c1 != -1 && c2 != -1) {         // colon separator found
                    iname = out.trim(token.substr(0, c1));  // extract name
                    iamount = out.trim(token.substr(c1 + 1, c2 - c1 - 1));  // extract amount
                    iunit = out.trim(token.substr(c2 + 1)); // extract unit
                } else {
                    iname = out.trim(token);        // only name provided
                }
            }
            double need = 0.0; if (iamount != "") { try { need = stod(iamount); } catch (...) { need = 0.0; } }  // parse required amount
            string unit = iunit;                    // store unit

            vector<Pantry> pantry = Pantry::loadAll();      // load pantry items
            double have = 0.0; string haveUnit = ""; bool found = false;
            string inameLower = out.toLowerCase(iname);     // normalize ingredient name
            for (int k = 0; k < pantry.size(); k++) {
                string pnamel = out.toLowerCase(pantry[k].name);    // normalize pantry name
                if (pnamel == inameLower) {         // match found
                    found = true;                   // mark as found
                    haveUnit = pantry[k].unit;      // get pantry unit
                    try { have = stod(pantry[k].quantity); } catch (...) { have = 0.0; }    // parse pantry quantity
                    break;                          // exit loop
                }
            }

            if (!found) {                           // ingredient not in pantry
                string qty = iamount == "" ? "1" : iamount;     // use amount or default to 1
                GroceryItem gi(iname, qty, unit);   // create grocery item
                gi.save();                          // save to grocery list
            } else {
                if (unit != "" && haveUnit != "" && unit != haveUnit) {     // unit mismatch
                    string qty2 = iamount == "" ? "1" : iamount;    // use amount or default to 1
                    GroceryItem gi2(iname, qty2, unit);     // create grocery item
                    gi2.save();                     // save to grocery list (no conversion)
                } else {
                    double diff = need - have;      // calculate difference
                    if (need == 0.0) {              // unknown amount
                        if (have <= 0.0) {          // pantry has none
                            GroceryItem gi3(iname, "1", unit);  // add 1 to list
                            gi3.save();             // save to grocery list
                        }
                    } else if (diff > 0.0) {        // need more than have
                        string addq = to_string(diff);      // convert difference to string
                        GroceryItem gi4(iname, addq, unit); // create grocery item
                        gi4.save();                 // save to grocery list
                    }
                }
            }
        }
    }

protected:
    /*
        Main modal schema - Generate grocery list from recipe workflow
    */
    void schema() override {
        listRecipes();                              // display all recipes
        if (recipes.size() == 0) { return; }        // exit if no recipes
        int n = out.inputi("Select recipe number to generate grocery list: ");  // prompt for selection
        if (n <= 0 || n > recipes.size()) {         // invalid selection
            out.coutln("Invalid selection.");       // display error
            out.br();
            return;                                 // exit modal
        }
        Recipe r = recipes[n - 1];                  // get selected recipe
        out.br();
        out.coutln("Generating grocery list for: " + r.name);   // display recipe name
        out.br();
        addMissingForRecipe(r);                     // generate grocery items
        out.coutln("Grocery list generation completed.");
        out.br();
    }
};
