#ifndef RECIPE_H
#define RECIPE_H

#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include "../../vendor/sysfunc/out.h"

using namespace std;

struct Recipe {
    string name;
    vector<string> ingredients;
    string instructions;

    // Convert recipe to JSON string
    string toJson() const {
        string json = "{\n";
        json += "    \"name\": \"" + out.escapeJson(name) + "\",\n";
        json += "    \"ingredients\": [";
        for (size_t i = 0; i < ingredients.size(); ++i) {
            json += "\"" + out.escapeJson(ingredients[i]) + "\"";
            if (i < ingredients.size() - 1) json += ", ";
        }
        json += "],\n";
        json += "    \"instructions\": \"" + out.escapeJson(instructions) + "\"\n";
        json += "  }";
        return json;
    }

    // Parse recipe from simple JSON (assumes well-formed input)
    static Recipe fromJson(const string& json) {
        Recipe r;
        r.name = out.extractJsonString(json, "name");
        r.instructions = out.extractJsonString(json, "instructions");
        r.ingredients = out.extractJsonArray(json, "ingredients");
        return r;
    }

    // Load all recipes from a JSON file
    static vector<Recipe> loadFromFile(const string& filename) {
        vector<Recipe> recipes;
        ifstream file(filename);
        if (!file.is_open()) return recipes;

        stringstream buffer;
        buffer << file.rdbuf();
        string content = buffer.str();
        file.close();

        // Simple JSON array parsing
        size_t pos = content.find("[");
        if (pos == string::npos) return recipes;
        size_t end = content.rfind("]");
        if (end == string::npos) return recipes;

        // Find each recipe object
        size_t i = pos + 1;
        while (i < end) {
            while (i < end && (content[i] == ' ' || content[i] == '\n' || content[i] == ',')) ++i;
            if (i >= end) break;
            if (content[i] == '{') {
                size_t objStart = i;
                int braceCount = 0;
                while (i < end) {
                    if (content[i] == '{') ++braceCount;
                    else if (content[i] == '}') {
                        --braceCount;
                        if (braceCount == 0) break;
                    }
                    ++i;
                }
                if (braceCount == 0) {
                    string recipeJson = content.substr(objStart, i - objStart + 1);
                    recipes.push_back(Recipe::fromJson(recipeJson));
                }
                ++i;
            } else {
                ++i;
            }
        }
        return recipes;
    }

    // Save all recipes to a JSON file
    static bool saveToFile(const string& filename, const vector<Recipe>& recipes) {
        ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }

        file << "[\n";
        for (size_t i = 0; i < recipes.size(); ++i) {
            file << recipes[i].toJson();
            if (i < recipes.size() - 1) file << ",";
            file << "\n";
        }
        file << "]\n";
        file.close();
        return true;
    }
};

// Utility functions for string manipulation
class RecipeUtils {
public:
    static string ltrim(const string& s) {
        size_t i = 0;
        while (i < s.size() && isspace(static_cast<unsigned char>(s[i]))) ++i;
        return s.substr(i);
    }

    static string rtrim(const string& s) {
        size_t i = s.size();
        while (i > 0 && isspace(static_cast<unsigned char>(s[i-1]))) --i;
        return s.substr(0, i);
    }

    static string trim(const string& s) {
        return rtrim(ltrim(s));
    }

    static string toLower(string s) {
        transform(s.begin(), s.end(), s.begin(),
            [](unsigned char c){ return static_cast<char>(tolower(c)); });
        return s;
    }

    static vector<string> splitCsv(const string& s) {
        vector<string> parts;
        string cur;
        for (char c : s) {
            if (c == ',') {
                parts.push_back(trim(cur));
                cur.clear();
            } else {
                cur += c;
            }
        }
        parts.push_back(trim(cur));
        // Remove empty entries
        parts.erase(remove_if(parts.begin(), parts.end(),
            [](const string& p){ return p.empty(); }), parts.end());
        return parts;
    }
};

#endif // RECIPE_H
