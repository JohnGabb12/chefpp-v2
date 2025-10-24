#include "../../vendor/sys/out.h"
#include "../../vendor/base/page.h"
#include "recipe.cpp"
#include "viewrecipe.cpp"
#include "deleterecipe.cpp"
#include <vector>
#include <string>
#include <algorithm>

/*
    SearchRecipePage Class

    This page handles searching recipes by name or ingredients.
    It displays search results and allows viewing individual recipes.

    How it works:
        - User selects search type (by name, by ingredients, or by id)
        - Enters search query
        - System searches through all recipes in CSV file
        - Displays matching recipes in a list
        - User can view a recipe by entering its number
        - Options: View Recipe, Delete Recipe, Research (search again), List All, or Back

    Header classes:
    #include "../../vendor/sys/out.h"
    #include "../../vendor/base/page.h"
    #include "recipe.cpp"
    #include "viewrecipe.cpp"

    SearchRecipePage:
        private:
            - searchResults         : Vector of recipes matching search query
            - lastSearchQuery       : Stores the last search query string
            - lastSearchType        : Stores the last search type (1=name, 2=ingredients, 3=id)
            - performSearch()       : Executes search based on type and query
            - searchByName()        : Searches recipes by name
            - searchByIngredients() : Searches recipes by ingredients
            - searchById()          : Finds a recipe by its numeric id
            - listAll()             : Loads all recipes into the results list
            - displayResults()      : Shows search results list
            - viewRecipeByNumber()  : Opens recipe modal by list number
            - deleteRecipeByNumber(): Deletes recipe from list by number
        protected:
            - schema()              : Main page logic (override from Page)
        public:
            - SearchRecipePage()    : Constructor
*/
class SearchRecipePage : public Page {
private:
    vector<Recipe> searchResults;                                           // stores matching recipes
    string lastSearchQuery;                                                 // last search query
    int lastSearchType;                                                     // last search type (1=name, 2=ingredients)

    /*
        Search recipes by name
    */
    void searchByName(string query) {
        vector<Recipe> allRecipes = Recipe::loadAll();                      // load all recipes from CSV
        searchResults.clear();                                              // clear previous results
        string lowerQuery = out.toLowerCase(query);                         // convert query to lowercase

        for (int i = 0; i < allRecipes.size(); i++) {                       // loop through all recipes
            string lowerName = out.toLowerCase(allRecipes[i].name);         // convert recipe name to lowercase
            if (lowerName.find(lowerQuery) != string::npos) {               // check if query is in name
                searchResults.push_back(allRecipes[i]);                     // add to results
            }
        }
    }

    /*
        Search recipes by ingredients
    */
    void searchByIngredients(string query) {
        vector<Recipe> allRecipes = Recipe::loadAll();                      // load all recipes from CSV
        searchResults.clear();                                              // clear previous results
        string lowerQuery = out.toLowerCase(query);                         // convert query to lowercase

        for (int i = 0; i < allRecipes.size(); i++) {                       // loop through all recipes
            bool found = false;                                             // flag for ingredient match
            for (int j = 0; j < allRecipes[i].ingredients.size(); j++) {    // loop through ingredients
                string lowerIngredient = out.toLowerCase(allRecipes[i].ingredients[j]);  // convert to lowercase
                if (lowerIngredient.find(lowerQuery) != string::npos) {     // check if query is in ingredient
                    found = true;                                           // mark as found
                    break;                                                  // stop searching this recipe
                }
            }
            if (found) {                                                    // if ingredient matched
                searchResults.push_back(allRecipes[i]);                     // add to results
            }
        }
    }

    /*
        List all recipes (no filtering)
    */
    void listAll() {
        searchResults = Recipe::loadAll();                                  // load all recipes
        lastSearchQuery = "all";                                           // mark query as all
    }

    /*
        Search recipe by numeric id (matches Recipe.id)
    */
    void searchById(int id) {
        vector<Recipe> allRecipes = Recipe::loadAll();                      // load all recipes from CSV
        searchResults.clear();                                              // clear previous results
        for (int i = 0; i < allRecipes.size(); i++) {                       // iterate all recipes
            if (allRecipes[i].id == id) {                                   // match on id
                searchResults.push_back(allRecipes[i]);                     // add match
                break;                                                      // stop after first match
            }
        }
    }

    /*
        Perform search based on type and query
    */
    void performSearch() {
        out.clear();                                                        // clear screen
        out.header("Search Recipe");                                        // show header
        out.br();                                                           // blank line
        out.coutln("Search by:");                                           // show search options
        out.coutln("1. Recipe name");                                       // option 1
        out.coutln("2. Ingredients");                                       // option 2
        out.coutln("3. ID");                                                // option 3 (by id)
        out.coutln("4. List All");                                          // option 4 (list all)
        out.coutln("5. Cancel");
        out.br();                                                           // blank line
        lastSearchType = out.inputi("Enter your choice: ");                 // get search type
        out.br();                                                           // blank line

        if (lastSearchType == 1) {                                          // search by name
            lastSearchQuery = out.inputs("Enter recipe name: ");            // get search query
            searchByName(lastSearchQuery);                                  // perform name search
        } else if (lastSearchType == 2) {                                   // search by ingredients
            lastSearchQuery = out.inputs("Enter ingredient: ");             // get search query
            searchByIngredients(lastSearchQuery);                           // perform ingredient search
        } else if (lastSearchType == 3) {                                   // search by id
            int id = out.inputi("Enter recipe id (number): ");             // get id to search
            searchById(id);                                                 // perform id search
            lastSearchQuery = "id=" + to_string(id);                        // record query text
        } else if (lastSearchType == 4) {                                   // list all
            listAll();                                                      // load all recipes into results
        } else {                                                            // invalid choice
            out.coutln("Invalid choice!");                                  // error message
            out.br();                                                       // blank line
            return;                                                         // exit function
        }
    }

    /*
        Display search results
    */
    void displayResults() {
        out.br();                                                           // blank line
        out.hr();                                                           // horizontal rule
        out.br();                                                           // blank line

        if (searchResults.size() == 0) {                                    // no results found
            out.coutln("No recipes found matching your search.");           // show message
        } else {                                                            // results found
            out.coutln("Search Results (" + to_string(searchResults.size()) + " recipe(s) found):");  // show count
            out.br();                                                       // blank line
            for (int i = 0; i < searchResults.size(); i++) {                // loop through results
                out.coutln(to_string(i + 1) + ". " + searchResults[i].name);  // display recipe name
            }
        }
    }

    /*
        View a recipe by its number in the results list
    */
    void viewRecipeByNumber() {
        if (searchResults.size() == 0) {                                    // no results to view
            out.coutln("No recipes to view. Please search first.");         // error message
            out.br();                                                       // blank line
            return;                                                         // exit function
        }

        out.br();                                                           // blank line
        int choice = out.inputi("Which recipe? (Enter number): ");          // get recipe number
        
        if (choice > 0 && choice <= searchResults.size()) {                 // valid choice
            ViewRecipeModal viewModal(searchResults[choice - 1]);           // create modal with recipe
            viewModal.init();                                               // show recipe modal
        } else {                                                            // invalid choice
            out.coutln("Invalid recipe number!");                           // error message
            out.br();                                                       // blank line
        }
    }

    /*
        Delete a recipe by its number in the results list
    */
    void deleteRecipeByNumber() {
        if (searchResults.size() == 0) {                                    // no results to delete
            out.coutln("No recipes to delete. Please search first.");       // error message
            out.br();                                                       // blank line
            return;                                                         // exit function
        }

        out.br();                                                           // blank line
        int choice = out.inputi("Which recipe to delete? (Enter number): ");  // get recipe number
        
        if (choice > 0 && choice <= searchResults.size()) {                 // valid choice
            Recipe recipeToDelete = searchResults[choice - 1];              // get selected recipe
            DeleteRecipeModal deleteModal(recipeToDelete.id);               // create modal with recipe id
            deleteModal.init();                                             // show delete modal
            performSearch();                                                // refresh search results after deletion
        } else {                                                            // invalid choice
            out.coutln("Invalid recipe number!");                           // error message
            out.br();                                                       // blank line
        }
    }

protected:
    /*
        Main page schema - Display search results
    */
    void schema() override {
        if (searchResults.size() == 0 && lastSearchQuery.empty()) {         // first time on page
            performSearch();                                                // perform initial search
        }
        displayResults();                                                   // show search results
    }

public:
    /*
        Constructor - Initialize page settings
    */
    SearchRecipePage() : Page() {
        this->header = "Search Recipe";                                     // set page header
        this->showheader = true;                                            // show header
        this->useExitInsteadOfBack = false;                                 // show "Back" option
        lastSearchType = 0;                                                 // initialize search type
        lastSearchQuery = "";                                               // initialize search query

        // Initialize options vector
        options = {
            Options("View Recipe", [this]() {
                viewRecipeByNumber();                                       // view selected recipe
            }),
            Options("Delete Recipe", [this]() {
                deleteRecipeByNumber();                                     // delete selected recipe
            }),
            Options("Research", [this]() {
                performSearch();                                            // perform new search
            }),
            Options("List All", [this]() {
                listAll();                                                  // load all recipes
            })
        };
    }
};
