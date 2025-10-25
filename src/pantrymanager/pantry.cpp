#ifndef PANTRY_H                                // for no dup def
#define PANTRY_H

#include "../../vendor/sys/out.h"
#include "../../vendor/sys/csv.h"
#include <vector>
#include <string>

using namespace std;

/*
    Pantry Struct

    This struct represents a pantry ingredient with its quantity and unit.
    It provides methods to save, load, update, and delete ingredients from the pantry CSV.

    How it works:
        - Each ingredient has an id, name, quantity, and unit
        - The struct provides CSV conversion methods for persistence
        - Static methods handle loading all ingredients and finding by id
        - Instance methods handle saving and updating quantities

    Header classes:
    #include "../../vendor/sys/out.h"
    #include "../../vendor/sys/csv.h"

    Pantry:
        public:
            - id                        : Unique ingredient id (1-based)
            - name                      : Ingredient name
            - quantity                  : Quantity value
            - unit                      : Unit of measurement (cups, g, tsp, etc)
            - displayPreview()          : Display ingredient with formatted box
            - toCSVRow()                : Convert ingredient to CSV row format
            - fromCSVRow()              : Create Pantry from CSV row (static)
            - save()                    : Save ingredient to CSV file (adds to existing if duplicate)
            - loadAll()                 : Load all ingredients from CSV file (static)
            - findById()                : Find and return ingredient by id (static)
            - findByName()              : Find and return ingredient by name (static)
            - deleteById()              : Delete ingredient by id from CSV (static)
            - updateQuantity()          : Update ingredient quantity in CSV
*/
struct Pantry {
public:
    int id;
    string name;
    string quantity;
    string unit;

    Pantry() { id = 0; }                            // default constructor

    Pantry(string n, string q, string u)
        : name(n), quantity(q), unit(u) { id = 0; } // constructor with name, quantity, unit

    Pantry(int i, string n, string q, string u)
        : id(i), name(n), quantity(q), unit(u) {}   // constructor with all fields

    /*
        Display ingredient preview with formatted box
    */
    void displayPreview() {
        out.coutln("+-----------------------------------------+");
        out.br();
        if (id > 0) {                               // if id is set
            out.coutln("ID: " + to_string(id));     // display id
        }
        out.coutln("Ingredient: " + name);          // display name
        out.coutln("Quantity: " + quantity + " " + unit);   // display quantity with unit
        out.br();
        out.coutln("+-----------------------------------------+");
        out.br();
    }

    /*
        Convert ingredient to CSV row format
    */
    vector<string> toCSVRow() {
        vector<string> row;
        row.push_back(to_string(id));               // add id to row
        row.push_back(name);                        // add name to row
        row.push_back(quantity);                    // add quantity to row
        row.push_back(unit);                        // add unit to row
        return row;                                 // return csv row
    }

    /*
        Create Pantry from CSV row
    */
    static Pantry fromCSVRow(vector<string> row) {
        Pantry item;
        if (row.size() >= 4) {                      // check valid row format
            int parsedId = 0;
            try {
                parsedId = stoi(row[0]);            // parse id from string
            } catch (...) {
                parsedId = 0;                       // default to 0 if parse fails
            }
            item.id = parsedId;                     // set id
            item.name = row[1];                     // set name
            item.quantity = row[2];                 // set quantity
            item.unit = row[3];                     // set unit
        }
        return item;                                // return pantry item
    }

    /*
        Save ingredient to CSV file
        If ingredient already exists, adds to existing quantity instead of creating duplicate
    */
    void save() {
        CSV csv("pantry.csv");                      // create csv object
        vector<vector<string>> data = csv.read();   // read existing data

        Pantry existing = findByName(name);         // check for duplicate
        if (existing.id > 0) {                      // duplicate found
            double existingQty = 0.0;
            double newQty = 0.0;
            
            try { existingQty = stod(existing.quantity); } catch (...) { existingQty = 0.0; }  // parse existing quantity
            try { newQty = stod(quantity); } catch (...) { newQty = 0.0; }                     // parse new quantity
            
            double totalQty = existingQty + newQty; // add quantities
            string totalQtyStr = to_string(totalQty);   // convert to string
            
            for (int i = 0; i < data.size(); i++) {
                if (data[i].size() >= 4) {          // valid row
                    int rowId = 0;
                    try { rowId = stoi(data[i][0]); } catch (...) { rowId = 0; }   // parse row id
                    if (rowId == existing.id) {     // found matching row
                        data[i][2] = totalQtyStr;   // update quantity
                        break;                      // exit loop
                    }
                }
            }
            
            if (csv.write(data)) {                  // write updated data
                out.coutln("Ingredient '" + name + "' already exists!");
                out.coutln("Updated quantity from " + existing.quantity + " " + existing.unit + " to " + totalQtyStr + " " + existing.unit);
            } else {
                out.coutln("Error: Could not update ingredient.");
            }
            out.br();
            return;                                 // exit method
        }

        int maxId = 0;                              // find max id for auto-increment
        for (int i = 0; i < data.size(); i++) {
            if (data[i].size() >= 1) {              // valid row
                int parsed = 0;
                try { parsed = stoi(data[i][0]); } catch (...) { parsed = 0; }     // parse id
                if (parsed > maxId) { maxId = parsed; }     // update max id
            }
        }
        id = maxId + 1;                             // set new id

        data.push_back(toCSVRow());                 // add new row

        if (csv.write(data)) {                      // write to file
            out.coutln("Ingredient '" + name + "' added successfully!");
        } else {
            out.coutln("Error: Could not save ingredient.");
        }
        out.br();
    }

    /*
        Load all ingredients from CSV file
    */
    static vector<Pantry> loadAll() {
        vector<Pantry> items;                       // create empty vector
        CSV csv("pantry.csv");                      // create csv object
        vector<vector<string>> data = csv.read();   // read csv data

        for (int i = 0; i < data.size(); i++) {
            if (!data[i].empty()) {                 // valid row
                items.push_back(fromCSVRow(data[i]));   // convert and add to items
            }
        }

        return items;                               // return all items
    }

    /*
        Find and return ingredient by id
    */
    static Pantry findById(int rid) {
        vector<Pantry> all = loadAll();             // load all ingredients
        for (int i = 0; i < all.size(); i++) {
            if (all[i].id == rid) {                 // match found
                return all[i];                      // return ingredient
            }
        }
        return Pantry();                            // return empty if not found
    }

    /*
        Find and return ingredient by name (case-insensitive)
    */
    static Pantry findByName(string searchName) {
        vector<Pantry> all = loadAll();             // load all ingredients
        
        string lowerSearchName = searchName;        // create lowercase copy
        for (int i = 0; i < lowerSearchName.length(); i++) {
            lowerSearchName[i] = tolower(lowerSearchName[i]);   // convert to lowercase
        }
        
        for (int i = 0; i < all.size(); i++) {
            string lowerItemName = all[i].name;     // create lowercase copy
            for (int j = 0; j < lowerItemName.length(); j++) {
                lowerItemName[j] = tolower(lowerItemName[j]);   // convert to lowercase
            }
            
            if (lowerItemName == lowerSearchName) { // match found
                return all[i];                      // return ingredient
            }
        }
        return Pantry();                            // return empty if not found
    }

    /*
        Delete ingredient by id from CSV file
    */
    static bool deleteById(int id) {
        CSV csv("pantry.csv");                      // create csv object
        vector<vector<string>> data = csv.read();   // read existing data
        vector<vector<string>> newData;             // create new data vector
        bool found = false;

        for (int i = 0; i < data.size(); i++) {
            if (data[i].size() >= 4) {              // valid row
                int rowId = 0;
                try {
                    rowId = stoi(data[i][0]);       // parse row id
                } catch (...) {
                    rowId = 0;                      // default to 0 if parse fails
                }
                if (rowId == id) {                  // match found
                    found = true;                   // mark as found
                    continue;                       // skip adding to newData
                }
            }
            newData.push_back(data[i]);             // add row to new data
        }

        if (found) {                                // ingredient was found and removed
            csv.write(newData);                     // write updated data
            return true;                            // success
        }
        return false;                               // not found
    }

    /*
        Update ingredient quantity in CSV file
    */
    bool updateQuantity(string newQuantity) {
        CSV csv("pantry.csv");                      // create csv object
        vector<vector<string>> data = csv.read();   // read existing data
        bool found = false;

        for (int i = 0; i < data.size(); i++) {
            if (data[i].size() >= 4) {              // valid row
                int rowId = 0;
                try {
                    rowId = stoi(data[i][0]);       // parse row id
                } catch (...) {
                    rowId = 0;                      // default to 0 if parse fails
                }
                if (rowId == id) {                  // match found
                    data[i][2] = newQuantity;       // update quantity in data
                    quantity = newQuantity;         // update object quantity
                    found = true;                   // mark as found
                    break;                          // exit loop
                }
            }
        }

        if (found) {                                // ingredient was found and updated
            csv.write(data);                        // write updated data
            return true;                            // success
        }
        return false;                               // not found
    }
};

#endif                                              // PANTRY_H
