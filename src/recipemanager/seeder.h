#ifndef SEEDER_H
#define SEEDER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

/*
    Seeder

    This header-only Seeder provides a single convenience method:
        Seeder::seedRecipes(filename, count)

    Behavior:
        - Ensures the ./data directory exists (best-effort).
        - Writes `count` unique sample recipes to the CSV file named by
          `filename` (example: "./data/recipes.csv").
        - Each recipe row uses the project's CSV format: id,name,ingredients,instructions
        - Ingredients are semicolon-separated; ingredient tokens use the
          normalized "name|amount|unit" form when applicable.

    Notes:
        - This file uses `using namespace std;` per project convention.
        - No explicit typecasts are used anywhere in this file.
        - The generated recipes are deterministic and unique for ids 1..count.
*/

class Seeder {
public:
    static bool seedRecipes(string filename, int count = 100) {
#if defined(_WIN32) || defined(_WIN64)
        system("if not exist .\\data mkdir .\\data");
#else
        system("mkdir -p ./data");
#endif

        ofstream file(filename, ios::trunc);
        if (!file.is_open()) return false;

        auto escapeField = [](string field) -> string {
            string s = field;
            // double quotes
            size_t pos = 0;
            while (true) {
                size_t found = s.find('"', pos);
                if (found == string::npos) break;
                s.insert(found, "\"");
                pos = found + 2;
            }
            bool needQuote = false;
            for (size_t i = 0; i < s.length(); ++i) {
                char c = s[i];
                if (c == ',' || c == '"' || c == '\n' || c == '\r') { needQuote = true; break; }
            }
            if (needQuote) return string("\"") + s + string("\"");
            return s;
        };

        // A collection of real-like recipes. Each entry contains realistic
        // ingredient lists and brief instructions. We combine these arrays
        // deterministically to produce 100 unique recipes.
        vector<string> names = {
            "Classic Pancakes",
            "Spaghetti Bolognese",
            "Chicken Stir-Fry",
            "Beef Tacos",
            "Vegetable Curry",
            "Grilled Salmon",
            "Tomato Basil Soup",
            "Caesar Salad",
            "Lentil Stew",
            "Shrimp Scampi",
            "Quinoa Salad",
            "BBQ Pulled Pork",
            "Mushroom Risotto",
            "Fajita Bowl",
            "Greek Salad",
            "Shepherd's Pie",
            "Chili Con Carne",
            "Egg Fried Rice",
            "Pesto Pasta",
            "Roast Chicken",
            "Fish and Chips",
            "Caprese Sandwich",
            "Butternut Squash Soup",
            "Stuffed Peppers",
            "Baked Ziti"
        };

        vector<string> ingredientsList = {
            "flour|2|cups; milk|1.5|cups; egg|1|unit; sugar|2|tbsp; baking powder|2|tsp",
            "spaghetti|200|g; ground beef|300|g; tomato sauce|1|cup; onion|1|unit; garlic|2|cloves",
            "chicken breast|300|g; soy sauce|2|tbsp; broccoli|150|g; bell pepper|1|unit; garlic|2|cloves",
            "tortillas|8|unit; ground beef|400|g; taco seasoning|2|tbsp; lettuce|1|cup; tomato|1|unit",
            "chickpeas|1|cup; coconut milk|1|cup; curry powder|2|tbsp; spinach|2|cups; onion|1|unit",
            "salmon fillet|2|pieces; lemon|1|unit; olive oil|2|tbsp; garlic|1|clove; salt|to taste",
            "tomato|4|unit; vegetable stock|4|cups; basil|1|handful; onion|1|unit; cream|0.5|cup",
            "romaine lettuce|1|head; croutons|1|cup; parmesan|0.5|cup; caesar dressing|0.5|cup",
            "lentils|1|cup; carrot|2|unit; celery|2|stalks; tomato paste|2|tbsp; stock|4|cups",
            "shrimp|300|g; butter|3|tbsp; garlic|3|cloves; lemon|1|unit; parsley|2|tbsp",
            "quinoa|1|cup; cucumber|1|unit; cherry tomatoes|1|cup; feta|0.5|cup; olive oil|2|tbsp",
            "pork shoulder|1|kg; bbq sauce|1|cup; onion|1|unit; brown sugar|2|tbsp",
            "arborio rice|1|cup; mushrooms|200|g; parmesan|0.5|cup; white wine|0.5|cup; stock|4|cups",
            "chicken|300|g; bell pepper|1|unit; onion|1|unit; fajita seasoning|2|tbsp; rice|1|cup",
            "cucumber|1|unit; tomato|2|unit; feta|0.5|cup; olive oil|2|tbsp; oregano|1|tsp",
            "ground lamb|500|g; potatoes|4|unit; peas|1|cup; onion|1|unit; gravy|1|cup",
            "ground beef|500|g; beans|1|cup; tomato|2|unit; chili powder|2|tbsp; onion|1|unit",
            "rice|2|cups; egg|2|unit; peas|1|cup; soy sauce|2|tbsp; sesame oil|1|tbsp",
            "pasta|200|g; pesto sauce|0.5|cup; pine nuts|2|tbsp; parmesan|0.25|cup",
            "whole chicken|1|unit; rosemary|1|sprig; lemon|1|unit; olive oil|2|tbsp; salt|to taste",
            "cod|2|fillets; potatoes|3|unit; flour|1|cup; oil|for frying; salt|to taste",
            "ciabatta|2|slices; tomato|2|unit; mozzarella|0.5|cup; basil|a few leaves; olive oil|1|tbsp",
            "butternut squash|1|unit; onion|1|unit; stock|4|cups; cream|0.5|cup; nutmeg|a pinch",
            "bell pepper|2|unit; rice|1|cup; ground beef|300|g; tomato|1|unit; cheese|0.5|cup",
            "ziti|300|g; tomato sauce|2|cups; ricotta|1|cup; mozzarella|1|cup; basil|1|tsp"
        };

        vector<string> instructionsList = {
            "Whisk dry ingredients, add milk and egg, cook on a hot griddle until golden.",
            "Cook spaghetti until al dente. Brown beef with onion and garlic, add tomato sauce and simmer.",
            "Slice chicken and vegetables, stir-fry over high heat with soy and garlic until cooked.",
            "Cook beef with seasoning, warm tortillas, assemble with lettuce and tomato.",
            "Sauté onion, add spices and chickpeas, pour coconut milk and simmer until thick.",
            "Season salmon, sear skin-side down, finish with lemon and a drizzle of oil.",
            "Simmer tomatoes and stock, blend until smooth, finish with cream and basil.",
            "Toss lettuce with dressing, top with croutons and parmesan, serve chilled.",
            "Sauté base vegetables, add lentils and stock, simmer until tender.",
            "Sauté garlic in butter, add shrimp and cook until pink, finish with lemon and parsley.",
            "Cook quinoa, chop vegetables, toss with feta and dressing, serve cold.",
            "Roast the pork slowly until tender, shred and mix with BBQ sauce.",
            "Toast rice, deglaze with wine, add stock gradually, stir until creamy, fold in mushrooms.",
            "Sear chicken and peppers with seasoning, serve over rice or in bowls.",
            "Combine chopped vegetables and feta, dress with olive oil and oregano.",
            "Layer meat and vegetables, top with mashed potatoes, bake until golden.",
            "Brown meat with spices, add beans and tomato, simmer to deepen flavors.",
            "Stir-fry cooked rice with eggs and peas, season with soy and sesame oil.",
            "Toss cooked pasta with pesto, top with pine nuts and parmesan.",
            "Roast whole chicken with herbs and lemon until juices run clear.",
            "Batter fish, fry until crisp; serve with fried potatoes and tartar sauce.",
            "Assemble tomato, mozzarella and basil on bread, drizzle olive oil and press.",
            "Roast squash with onion, simmer with stock and blend until smooth.",
            "Fill peppers with cooked rice and meat, top with cheese and bake.",
            "Mix ziti with sauces and cheeses, bake until bubbly and golden."
        };

        for (int id = 1; id <= count; ++id) {
            size_t idx = id - 1;
            string sid = to_string(id);

            string baseName = names[idx % names.size()];
            string name = baseName;
            
            // make name unique when cycling through base names
            if (id > names.size()) {
                int variant = (id - 1) / names.size();
                name = baseName + " Variant " + to_string(variant + 1);
            }

            string ingredients = ingredientsList[idx % ingredientsList.size()];
            string instructions = instructionsList[idx % instructionsList.size()];

            vector<string> row;
            row.push_back(sid);
            row.push_back(name);
            row.push_back(ingredients);
            row.push_back(instructions);

            for (size_t i = 0; i < row.size(); ++i) {
                if (i > 0) file << ',';
                file << escapeField(row[i]);
            }
            file << '\n';
        }

        file.close();
        return true;
    }
};

#endif // SEEDER_H
