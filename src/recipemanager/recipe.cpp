#ifndef RECIPE_H
#define RECIPE_H

#include "../../vendor/sys/out.h"
#include "../../vendor/sys/csv.h"
#include <vector>
#include <string>

using namespace std;

struct Recipe {
public:
    string name;
    vector<string> ingredients;
    string instructions;

    Recipe() {}
    
    Recipe(string n, vector<string> ing, string inst) 
        : name(n), ingredients(ing), instructions(inst) {}

    /*
        Display recipe preview with formatted box
    */
    void displayPreview() {
        out.coutln("+-----------------------------------------+");
        out.br();
        out.coutln("Recipe: " + name);
        out.br();
        out.coutln("Ingredients:");
        for (const string& ingredient : ingredients) {
            out.coutln("- " + ingredient);
        }
        out.br();
        out.coutln(instructions);
        out.br();
        out.coutln("+-----------------------------------------+");
        out.br();
    }

    /*
        Parse comma-separated ingredients string into vector
    */
    static vector<string> parseIngredients(string ingredientsInput) {
        vector<string> result;
        string current = "";
        
        for (char c : ingredientsInput) {
            if (c == ',') {
                string trimmed = out.trim(current);                         // trim whitespace
                if (!trimmed.empty()) {                                     // check if not empty after trim
                    result.push_back(trimmed);                              // add trimmed ingredient
                }
                current = "";                                               // reset current string
            } else {
                current += c;                                               // build current ingredient
            }
        }
        
        // Add last ingredient
        string trimmed = out.trim(current);                                 // trim whitespace
        if (!trimmed.empty()) {                                             // check if not empty after trim
            result.push_back(trimmed);                                      // add trimmed ingredient
        }
        
        return result;                                                      // return parsed ingredients
    }

    /*
        Convert recipe to CSV row format
    */
    vector<string> toCSVRow() {
        vector<string> row;
        row.push_back(name);                                                // add recipe name
        
        // Join ingredients with semicolon separator
        string ingredientsStr = "";
        for (size_t i = 0; i < ingredients.size(); i++) {
            if (i > 0) ingredientsStr += ";";                               // add semicolon separator
            ingredientsStr += ingredients[i];                               // add ingredient
        }
        row.push_back(ingredientsStr);                                      // add ingredients as one field
        
        row.push_back(instructions);                                        // add instructions
        return row;                                                         // return CSV row
    }

    /*
        Create Recipe from CSV row
    */
    static Recipe fromCSVRow(vector<string> row) {
        Recipe recipe;
        if (row.size() >= 3) {                                              // ensure row has all fields
            recipe.name = row[0];                                           // get recipe name
            
            // Parse ingredients (semicolon-separated)
            string ingredientsStr = row[1];
            string current = "";
            for (char c : ingredientsStr) {
                if (c == ';') {
                    if (!current.empty()) {
                        recipe.ingredients.push_back(out.trim(current));    // add trimmed ingredient
                        current = "";
                    }
                } else {
                    current += c;
                }
            }
            if (!current.empty()) {
                recipe.ingredients.push_back(out.trim(current));            // add last ingredient
            }
            
            recipe.instructions = row[2];                                   // get instructions
        }
        return recipe;                                                      // return recipe
    }

    /*
        Save recipe to CSV file
    */
    void save() {
        CSV csv("recipes.csv");                                             // open recipes CSV file
        
        // Read existing recipes
        vector<vector<string>> data = csv.read();                           // read all existing recipes
        
        // Add new recipe
        data.push_back(toCSVRow());                                         // add current recipe to data
        
        // Write back to file
        if (csv.write(data)) {                                              // write all recipes to file
            out.coutln("Recipe '" + name + "' added successfully!");        // success message
        } else {
            out.coutln("Error: Could not save recipe.");                    // error message
        }
        out.br();                                                           // add blank line
    }

    /*
        Load all recipes from CSV file
    */
    static vector<Recipe> loadAll() {
        vector<Recipe> recipes;                                             // create empty recipes vector
        CSV csv("recipes.csv");                                             // open recipes CSV file
        vector<vector<string>> data = csv.read();                           // read all rows
        
        for (int i = 0; i < data.size(); i++) {                             // loop through each row
            if (!data[i].empty() && !data[i][0].empty()) {                  // skip empty rows
                recipes.push_back(fromCSVRow(data[i]));                     // convert and add recipe
            }
        }
        
        return recipes;                                                     // return all recipes
    }
};

#endif // RECIPE_H