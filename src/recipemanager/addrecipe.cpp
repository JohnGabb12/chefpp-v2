#include "../../vendor/sys/out.h"
#include "../../vendor/base/modal.h"
#include "recipe.cpp"
#include <vector>

/*
    AddRecipeModal Class

    This modal handles adding new recipes to the system.
    It collects recipe data, displays a preview, and allows editing before saving.

    How it works:
        - Collects recipe name, ingredients, and instructions from user
        - Displays a preview of the recipe
        - Asks for confirmation (y/n)
        - If not confirmed, allows editing specific fields
        - Loops until confirmed, then saves to CSV file
        - Automatically returns to parent page after completion

    Header classes:
    #include "../../vendor/sys/out.h"
    #include "../../vendor/base/modal.h"
    #include "recipe.cpp"

    AddRecipeModal:
        private:
            - recipe                    : Recipe object being created
            - collectRecipeData()       : Prompts user for recipe information
            - confirmRecipe()           : Shows preview and asks for confirmation
            - changeRecipe()            : Allows editing specific recipe fields
        protected:
            - schema()                  : Main modal logic (override from Modal)
        public:
            - AddRecipeModal()          : Constructor
*/
class AddRecipeModal : public Modal {
private:
    Recipe recipe;                                                          // recipe being created

    /*
        Collect recipe data from user input
    */
    void collectRecipeData() {
        out.hr();                                                           // display separator
        out.br();
        recipe.name = out.inputs("Enter recipe name: ");                    // get recipe name
        string ingredientsInput = out.inputs("Enter ingredients (comma-separated; use name:amount:unit optional): ");  // get ingredients input with measurement format
        recipe.ingredients = Recipe::parseIngredients(ingredientsInput);    // parse ingredients to vector
        recipe.instructions = out.inputs("Enter instructions: ");           // get cooking instructions
        out.br();
    }

    /*
        Display recipe preview and ask for confirmation
    */
    bool confirmRecipe() {                                                     // blank line
        out.clear();                                                          // clear screen
        recipe.displayPreview();                                            // show recipe preview box
        return out.inputYesNo("Are you sure? (y/n): ");                     // get yes/no confirmation
    }

    /*
        Allow user to edit specific recipe fields
    */
    void changeRecipe() {
        out.br();                                                           // blank line
        out.coutln("What do you want to change:");                          // show edit menu
        out.coutln("1. Recipe name");                                       // option 1
        out.coutln("2. Ingredients");                                       // option 2
        out.coutln("3. Instructions");                                      // option 3
        out.coutln("4. None");                                              // option 4
        int choice = out.inputi("Enter your choice: ");                     // get user choice
        out.br();                                                           // blank line

        switch (choice) {                                                   // handle user choice
            case 1:                                                         // change recipe name
                recipe.name = out.inputs("Recipe name: ");                  // get new name
                out.coutln("Recipe name changed successfully!");            // success message
                out.br();                                                   // blank line
                break;
            case 2: {                                                       // change ingredients
                string ingredientsInput = out.inputs("Enter ingredients (comma-separated; use name:amount:unit optional): ");  // get new ingredients with measurement format
                recipe.ingredients = Recipe::parseIngredients(ingredientsInput);  // parse to vector
                out.coutln("Ingredients changed successfully!");            // success message
                out.br();                                                   // blank line
                break;
            }
            case 3:                                                         // change instructions
                recipe.instructions = out.inputs("Instructions: ");         // get new instructions
                out.coutln("Instructions changed successfully!");           // success message
                out.br();                                                   // blank line
                break;
            case 4:                                                         // no changes
                return;                                                     // exit function
            default:                                                        // invalid choice
                out.coutln("Invalid choice!");                              // error message
                break;
        }
    }

protected:
    /*
        Main modal schema - Add recipe workflow
    */
    void schema() override {
        collectRecipeData();                                                // collect recipe information
        
        while (true) {                                                      // loop until confirmed
            if (confirmRecipe()) {                                          // show preview and ask confirmation
                recipe.save();                                              // save recipe to CSV file
                out.coutln("Recipe saved successfully!");                   // success message
                out.br();                                                   // blank line
                recipe = Recipe();                                          // reset recipe for next add
                return;                                                     // exit to show options
            } else {                                                        // user wants to make changes
                changeRecipe();                                             // allow editing fields
            }
        }
    }
};
