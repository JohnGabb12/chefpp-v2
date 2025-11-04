#ifndef RECIPE_H
#define RECIPE_H

#include "../../vendor/sys/out.h"
#include "../../vendor/sys/csv.h"
#include <vector>
#include <string>

using namespace std;

/*
    Recipe data structure and helpers

    Purpose:
        Encapsulates a single recipe and provides utilities to
        - display a recipe preview to the console
        - parse ingredient lists supplied by the user
        - serialize/deserialize recipes to/from CSV rows
        - persist recipes to the `recipes.csv` file and load all recipes

    Data contract / CSV layout:
        Current CSV row format (4 columns):
            [id, name, ingredients, instructions]

        Legacy CSV row format (3 columns):
            [name, ingredients, instructions]
        The code detects legacy rows and migrates the file to include ids.

    Ingredients representation:
        Internally each ingredient is stored as a single string. The code
        normalizes optional measurement information using a pipe-delimited
        representation: "name|amount|unit" when amount/unit are present.

        Accepted input forms for an ingredient token (when parsing user input):
            - name
            - name:amount:unit   (colon-separated)
            - name|amount|unit   (pipe-separated)

        When writing to CSV the ingredients vector is joined with a semicolon
        separator into the `ingredients` field of the CSV row.
*/

struct Recipe {
public:
    int id;                                                                // unique recipe id (1-based)
    string name;
    vector<string> ingredients;
    string instructions;

    /*
        Constructors
            - Recipe()
                Default constructor. Sets id to 0 (unknown).

            - Recipe(string n, vector<string> ing, string inst)
                Create a recipe with the given name, ingredients and
                instructions. id remains 0 until saved.

            - Recipe(int i, string n, vector<string> ing, string inst)
                Create a recipe with an explicit id (used when loading
                existing recipes from CSV).
    */
    Recipe() { id = 0; }
    
    Recipe(string n, vector<string> ing, string inst) 
        : name(n), ingredients(ing), instructions(inst) { id = 0; }

    Recipe(int i, string n, vector<string> ing, string inst)
        : id(i), name(n), ingredients(ing), instructions(inst) {}

    /*
        displayPreview()

        Purpose:
            Print a nicely formatted preview of this Recipe to the console.

        Inputs:
            Uses the instance fields: id, name, ingredients, instructions.

        Outputs / Side-effects:
            Writes multiple lines to the global `out` console helper. No return value.

        Behavior details:
            - Displays an optional ID when id > 0.
            - Attempts to parse each ingredient token for measurement/unit
              information using either '|' (preferred) or ':' separators and
              formats the display as "- name (amount unit)" when available.

        Edge cases:
            - If ingredient tokens are not normalized, best-effort parsing is used.
    */
    void displayPreview() {
        out.coutln("+-----------------------------------------+");
        out.br();
        if (id > 0) {
            out.coutln("ID: " + to_string(id));                           // display id if available
        }
        out.coutln("Recipe: " + name);
        out.br();
        out.coutln("Ingredients:");
        for (int i = 0; i < ingredients.size(); i++) {                    // iterate over ingredients
            string ingredient = ingredients[i];                            // get ingredient token
            // Attempt to parse measurement and unit using '|' or ':' separators
            string iname = "";
            string iamount = "";
            string iunit = "";

            // First, check for '|' separated form: name|amount|unit
            int p1 = -1;
            int p2 = -1;
            for (int j = 0; j < ingredient.length(); j++) {               // find first '|'
                if (ingredient[j] == '|') { p1 = j; break; }
            }
            if (p1 != -1) {                                               // found first separator
                for (int j = p1 + 1; j < ingredient.length(); j++) {      // find second '|'
                    if (ingredient[j] == '|') { p2 = j; break; }
                }
            }

            if (p1 != -1 && p2 != -1) {                                   // full form present
                iname = out.trim(ingredient.substr(0, p1));
                iamount = out.trim(ingredient.substr(p1 + 1, p2 - p1 - 1));
                iunit = out.trim(ingredient.substr(p2 + 1));
            } else {
                // Try ':' separated form as fallback: name:amount:unit
                int c1 = -1;
                int c2 = -1;
                for (int j = 0; j < ingredient.length(); j++) {
                    if (ingredient[j] == ':') { c1 = j; break; }
                }
                if (c1 != -1) {
                    for (int j = c1 + 1; j < ingredient.length(); j++) {
                        if (ingredient[j] == ':') { c2 = j; break; }
                    }
                }
                if (c1 != -1 && c2 != -1) {
                    iname = out.trim(ingredient.substr(0, c1));
                    iamount = out.trim(ingredient.substr(c1 + 1, c2 - c1 - 1));
                    iunit = out.trim(ingredient.substr(c2 + 1));
                } else {
                    iname = out.trim(ingredient);                          // no measurement info
                }
            }

            // Build display line
            string line = "- " + iname;
            if (!iamount.empty() && !iunit.empty()) {
                line += " (" + iamount + " " + iunit + ")";             // amount and unit
            } else if (!iamount.empty()) {
                line += " (" + iamount + ")";                            // amount only
            }
            out.coutln(line);
        }
        out.br();
        out.coutln(instructions);
        out.br();
        out.coutln("+-----------------------------------------+");
        out.br();
    }

    /*
        parseIngredients(string ingredientsInput)

        Purpose:
            Convert a single comma-separated ingredients string provided by
            the user into a vector of normalized ingredient tokens.

        Input:
            - ingredientsInput: a string like
              "salt, flour|2|cups, sugar:1:cup"

        Output:
            - vector<string> where each element is either:
                - "name"
                - "name|amount|unit" (pipe-delimited when amount/unit present)

        Notes:
            - The parser prefers '|' separators but will accept ':' as a
              fallback. Empty tokens are ignored. Amounts are stored as
              raw strings (no numeric validation).
    */
    static vector<string> parseIngredients(string ingredientsInput) {
        vector<string> result;                                             // parsed ingredients
        string current = "";                                               // current token buffer

        // Split by comma into individual ingredient tokens
        for (int idx = 0; idx < ingredientsInput.length(); idx++) {
            char c = ingredientsInput[idx];
            if (c == ',') {
                string token = out.trim(current);
                if (!token.empty()) {
                    // Normalize token to support optional measurement and unit
                    // Accepted forms:
                    //   name
                    //   name:amount:unit
                    //   name|amount|unit
                    string nameToken = "";
                    string amountToken = "";
                    string unitToken = "";

                    // Prefer '|' separation if present
                    int p1 = -1; int p2 = -1;
                    for (int j = 0; j < token.length(); j++) { if (token[j] == '|') { p1 = j; break; } }
                    if (p1 != -1) { for (int j = p1 + 1; j < token.length(); j++) { if (token[j] == '|') { p2 = j; break; } } }

                    if (p1 != -1 && p2 != -1) {
                        nameToken = out.trim(token.substr(0, p1));
                        amountToken = out.trim(token.substr(p1 + 1, p2 - p1 - 1));
                        unitToken = out.trim(token.substr(p2 + 1));
                    } else {
                        // Fallback to ':' separation
                        int c1 = -1; int c2 = -1;
                        for (int j = 0; j < token.length(); j++) { if (token[j] == ':') { c1 = j; break; } }
                        if (c1 != -1) { for (int j = c1 + 1; j < token.length(); j++) { if (token[j] == ':') { c2 = j; break; } } }
                        if (c1 != -1 && c2 != -1) {
                            nameToken = out.trim(token.substr(0, c1));
                            amountToken = out.trim(token.substr(c1 + 1, c2 - c1 - 1));
                            unitToken = out.trim(token.substr(c2 + 1));
                        } else {
                            nameToken = token;                              // only name provided
                        }
                    }

                    // Store normalized representation
                    if (!amountToken.empty() || !unitToken.empty()) {
                        result.push_back(nameToken + "|" + amountToken + "|" + unitToken);
                    } else {
                        result.push_back(nameToken);
                    }
                }
                current = "";                                              // reset buffer
            } else {
                current += c;                                              // accumulate
            }
        }

        // Handle last token (no trailing comma)
        string lastToken = out.trim(current);
        if (!lastToken.empty()) {
            string nameToken2 = "";
            string amountToken2 = "";
            string unitToken2 = "";
            int p1b = -1; int p2b = -1;
            for (int j = 0; j < lastToken.length(); j++) { if (lastToken[j] == '|') { p1b = j; break; } }
            if (p1b != -1) { for (int j = p1b + 1; j < lastToken.length(); j++) { if (lastToken[j] == '|') { p2b = j; break; } } }
            if (p1b != -1 && p2b != -1) {
                nameToken2 = out.trim(lastToken.substr(0, p1b));
                amountToken2 = out.trim(lastToken.substr(p1b + 1, p2b - p1b - 1));
                unitToken2 = out.trim(lastToken.substr(p2b + 1));
            } else {
                int c1b = -1; int c2b = -1;
                for (int j = 0; j < lastToken.length(); j++) { if (lastToken[j] == ':') { c1b = j; break; } }
                if (c1b != -1) { for (int j = c1b + 1; j < lastToken.length(); j++) { if (lastToken[j] == ':') { c2b = j; break; } } }
                if (c1b != -1 && c2b != -1) {
                    nameToken2 = out.trim(lastToken.substr(0, c1b));
                    amountToken2 = out.trim(lastToken.substr(c1b + 1, c2b - c1b - 1));
                    unitToken2 = out.trim(lastToken.substr(c2b + 1));
                } else {
                    nameToken2 = lastToken;
                }
            }
            if (!amountToken2.empty() || !unitToken2.empty()) {
                result.push_back(nameToken2 + "|" + amountToken2 + "|" + unitToken2);
            } else {
                result.push_back(nameToken2);
            }
        }

        return result;                                                     // return parsed ingredients
    }

    /*
        toCSVRow()

        Purpose:
            Serialize this Recipe into a vector of strings representing one
            CSV row. The format used is [id, name, ingredients, instructions].

        Output:
            - vector<string> suitable for passing to CSV::write

        Notes:
            - Ingredients are joined with a semicolon (';') into a single
              field for storage.
    */
    vector<string> toCSVRow() {
        vector<string> row;
        row.push_back(to_string(id));                                       // add id as first column
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
        fromCSVRow(vector<string> row)

        Purpose:
            Construct a Recipe from a CSV row vector.

        Input formats supported:
            - New format (4 cols): [id, name, ingredients, instructions]
            - Legacy format (3 cols): [name, ingredients, instructions]

        Behavior:
            - Parses id safely (defaults to 0 on parse failure).
            - Splits the ingredients field on ';' and trims tokens.
    */
    static Recipe fromCSVRow(vector<string> row) {
        Recipe recipe;
        if (row.size() >= 4) {                                              // new format: id,name,ingredients,instructions
            int parsedId = 0;
            try {
                parsedId = stoi(row[0]);
            } catch (...) {
                parsedId = 0;
            }
            recipe.id = parsedId;
            recipe.name = row[1];                                           // get recipe name
            
            // Parse ingredients (semicolon-separated)
            string ingredientsStr = row[2];
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
            
            recipe.instructions = row[3];                                   // get instructions
        } else if (row.size() >= 3) {                                       // legacy format: name,ingredients,instructions
            recipe.id = 0;                                                  // id unknown in legacy row
            recipe.name = row[0];
            string ingredientsStr2 = row[1];
            string current2 = "";
            for (char c2 : ingredientsStr2) {
                if (c2 == ';') {
                    if (!current2.empty()) {
                        recipe.ingredients.push_back(out.trim(current2));
                        current2 = "";
                    }
                } else {
                    current2 += c2;
                }
            }
            if (!current2.empty()) {
                recipe.ingredients.push_back(out.trim(current2));
            }
            recipe.instructions = row[2];
        }
        return recipe;                                                      // return recipe
    }

    /*
        save()

        Purpose:
            Persist this Recipe to the `recipes.csv` file.

        Side-effects:
            - Reads the entire CSV file into memory.
            - Detects and migrates legacy 3-column rows to the 4-column
              format by assigning sequential ids.
            - Assigns this recipe an id (max existing id + 1) and appends
              the row to the in-memory data, then writes the file back.

        Notes / limitations:
            - No concurrency control; simultaneous saves may conflict.
            - Reports success/failure via console output only (no return value).
    */
    void save() {
        CSV csv("recipes.csv");                                             // open recipes CSV file

        // Read existing recipes
        vector<vector<string>> data = csv.read();                           // read all existing recipes

        // If legacy format (3 cols), migrate to include id as first column
        bool migrated = false;
        for (int mi = 0; mi < data.size(); mi++) {
            if (data[mi].size() == 3) { migrated = true; break; }
        }
        if (migrated) {
            vector<vector<string>> newdata;
            int nid = 1;
            for (int ri = 0; ri < data.size(); ri++) {
                if (data[ri].size() == 0) { continue; }
                vector<string> row4;
                row4.push_back(to_string(nid));         // id
                if (data[ri].size() >= 1) row4.push_back(data[ri][0]); else row4.push_back(""); // name
                if (data[ri].size() >= 2) row4.push_back(data[ri][1]); else row4.push_back(""); // ingredients
                if (data[ri].size() >= 3) row4.push_back(data[ri][2]); else row4.push_back(""); // instructions
                newdata.push_back(row4);
                nid++;
            }
            csv.write(newdata);                                            // write migrated data
            data = newdata;                                                // use migrated data
        }

        // Determine next id (max existing id + 1)
        int maxId = 0;
        for (int i = 0; i < data.size(); i++) {
            if (data[i].size() >= 1) {
                int parsed = 0;
                try { parsed = stoi(data[i][0]); } catch (...) { parsed = 0; }
                if (parsed > maxId) { maxId = parsed; }
            }
        }
        id = maxId + 1;                                                    // assign next id

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
        loadAll()

        Purpose:
            Read all recipes from `recipes.csv` and return them as a vector
            of Recipe objects.

        Side-effects:
            - If legacy rows are detected, the CSV file will be migrated to
              include ids and rewritten.

        Returns:
            - vector<Recipe> containing every non-empty row converted via
              fromCSVRow().
    */
    static vector<Recipe> loadAll() {
        vector<Recipe> recipes;                                             // create empty recipes vector
        CSV csv("recipes.csv");                                             // open recipes CSV file
        vector<vector<string>> data = csv.read();                           // read all rows

        // If legacy format detected, migrate file to include ids
        bool migrate = false;
        for (int mi = 0; mi < data.size(); mi++) {
            if (data[mi].size() == 3) { migrate = true; break; }
        }
        if (migrate) {
            vector<vector<string>> newdata2;
            int nid2 = 1;
            for (int r = 0; r < data.size(); r++) {
                if (data[r].size() == 0) { continue; }
                vector<string> rowx;
                rowx.push_back(to_string(nid2));
                if (data[r].size() >= 1) rowx.push_back(data[r][0]); else rowx.push_back("");
                if (data[r].size() >= 2) rowx.push_back(data[r][1]); else rowx.push_back("");
                if (data[r].size() >= 3) rowx.push_back(data[r][2]); else rowx.push_back("");
                newdata2.push_back(rowx);
                nid2++;
            }
            csv.write(newdata2);
            data = newdata2;
        }

        for (int i = 0; i < data.size(); i++) {                             // loop through each row
            if (!data[i].empty()) {                                         // skip empty rows
                recipes.push_back(fromCSVRow(data[i]));                     // convert and add recipe
            }
        }

        return recipes;                                                     // return all recipes
    }

    /*
        findById(int rid)

        Purpose:
            Locate and return a Recipe with the specified id.

        Behavior:
            - Loads all recipes (via loadAll()) and performs a linear search.
            - Returns the matching Recipe if found, otherwise returns a
              default Recipe() with id == 0.

        Complexity: O(N) where N is the number of recipes on disk.
    */
    static Recipe findById(int rid) {
        vector<Recipe> all = loadAll();                                    // load all recipes
        for (int i = 0; i < all.size(); i++) {                             // iterate recipes
            if (all[i].id == rid) {                                        // match on id
                return all[i];                                             // return recipe
            }
        }
        return Recipe();                                                   // not found
    }

    /*
        deleteById(int id)

        Purpose:
            Remove the recipe with the given id from the CSV file.

        Behavior / Side-effects:
            - Reads all rows, filters out the matching row by parsing the
              first column as an integer, and writes the filtered data back.
            - Returns true if a row was found and deleted, false otherwise.

        Notes:
            - Malformed id fields are treated as 0 and won't match typical
              positive ids. No concurrency handling is provided.
    */
    static bool deleteById(int id) {
        CSV csv("recipes.csv");                                             // open recipes CSV file
        vector<vector<string>> data = csv.read();                           // read all rows
        vector<vector<string>> newData;                                     // filtered data without deleted recipe
        bool found = false;                                                 // track if recipe was found

        for (int i = 0; i < data.size(); i++) {                             // loop through each row
            if (data[i].size() >= 4) {                                      // valid row with id
                int rowId = 0;
                try {
                    rowId = stoi(data[i][0]);                               // parse id from first column
                } catch (...) {
                    rowId = 0;
                }
                if (rowId == id) {                                          // found recipe to delete
                    found = true;                                           // mark as found
                    continue;                                               // skip this row (delete it)
                }
            }
            newData.push_back(data[i]);                                     // keep this row
        }

        if (found) {                                                        // recipe was found and removed
            csv.write(newData);                                             // write filtered data back to file
            return true;                                                    // deletion successful
        }
        return false;                                                       // recipe not found
    }
};

#endif // RECIPE_H