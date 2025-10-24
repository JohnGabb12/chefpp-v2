#ifndef DELETERECIPE_H
#define DELETERECIPE_H

#include "../../vendor/sys/out.h"
#include "../../vendor/base/modal.h"
#include "recipe.cpp"

/*
    DeleteRecipeModal Class

    This modal handles deleting recipes by their ID.
    It prompts for the recipe ID, displays the recipe for confirmation,
    and deletes it from the CSV file if confirmed.

    How it works:
        - Prompts user for recipe ID
        - Loads and displays the recipe with that ID
        - Asks for confirmation (y/n)
        - If confirmed, deletes the recipe from CSV file
        - Displays success or error message
        - Automatically returns to parent page after completion

    Header classes:
    #include "../../vendor/sys/out.h"
    #include "../../vendor/base/modal.h"
    #include "recipe.cpp"

    DeleteRecipeModal:
        private:
            - recipeId              : ID of recipe to delete
            - confirmDelete()       : Shows recipe and asks for confirmation
            - performDelete()       : Executes deletion from CSV
        protected:
            - schema()              : Main modal logic (override from Modal)
        public:
            - DeleteRecipeModal()   : Constructor (prompts for ID)
            - DeleteRecipeModal(int): Constructor with ID provided
*/
class DeleteRecipeModal : public Modal {
private:
    int recipeId;                                                           // id of recipe to delete

    // Removed local finder; use Recipe::findById(int) from Recipe class

    /*
        Display recipe and ask for deletion confirmation
    */
    bool confirmDelete(Recipe recipe) {
        if (recipe.id == 0) {                                               // recipe not found
            out.coutln("Error: Recipe with ID " + to_string(recipeId) + " not found.");  // error message
            out.br();                                                       // blank line
            return false;                                                   // cannot delete
        }

        out.coutln("Recipe to delete:");                                    // show header
        out.br();                                                           // blank line
        recipe.displayPreview();                                            // show recipe details
        return out.inputYesNo("Are you sure you want to delete this recipe? (y/n): ");  // get confirmation
    }

    /*
        Perform deletion from CSV file
    */
    void performDelete() {
        if (Recipe::deleteById(recipeId)) {                                 // attempt deletion
            out.coutln("Recipe deleted successfully!");                     // success message
        } else {                                                            // deletion failed
            out.coutln("Error: Could not delete recipe.");                  // error message
        }
        out.br();                                                           // blank line
    }

protected:
    /*
        Main modal schema - Delete recipe workflow
    */
    void schema() override {
    Recipe recipe = Recipe::findById(recipeId);                         // find recipe by id
        if (confirmDelete(recipe)) {                                        // show recipe and get confirmation
            performDelete();                                                // delete from CSV
        } else {                                                            // user cancelled or recipe not found
            out.coutln("Deletion cancelled.");                              // cancelled message
            out.br();                                                       // blank line
        }
    }

public:
    /*
        Constructor - Prompt for recipe ID
    */
    DeleteRecipeModal() : Modal() {
        out.clear();                                                        // clear screen
        out.header("Delete Recipe");                                        // show header
        out.br();                                                           // blank line
        recipeId = out.inputi("Enter recipe ID to delete: ");               // prompt for id
        out.br();                                                           // blank line
    }

    /*
        Constructor - ID provided (for use from search results)
    */
    DeleteRecipeModal(int id) : Modal() {
        recipeId = id;                                                      // store provided id
    }
};

#endif // DELETERECIPE_H
