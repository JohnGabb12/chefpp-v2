#ifndef GROCERY_H                               // for no dup def
#define GROCERY_H

#include "../../vendor/sys/out.h"
#include "../../vendor/sys/csv.h"
#include <vector>
#include <string>

using namespace std;

/*
    GroceryItem Struct

    Represents a grocery list item with name, quantity, and unit.
    CSV format: id,name,quantity,unit
    
    How it works:
        - Each item has an id, name, quantity, and unit
        - The struct provides CSV conversion methods for persistence
        - Static methods handle loading all items and finding by id or name+unit
        - Instance methods handle saving (with duplicate merging) and updating
        - Duplicate items (same name+unit) are merged by adding quantities
    
    Header classes:
    #include "../../vendor/sys/out.h"
    #include "../../vendor/sys/csv.h"
    
    GroceryItem:
        public:
            - id                        : Unique item id (1-based)
            - name                      : Item name
            - quantity                  : Quantity value
            - unit                      : Unit of measurement
            - toCSVRow()                : Convert item to CSV row format
            - fromCSVRow()              : Create GroceryItem from CSV row (static)
            - loadAll()                 : Load all items from CSV file (static)
            - findById()                : Find and return item by id (static)
            - findByNameAndUnit()       : Find item by name and unit (static)
            - save()                    : Save item to CSV (merge duplicates by name+unit)
            - deleteById()              : Delete item by id from CSV (static)
            - updateQuantityById()      : Update item quantity by id (static)
            - clearAll()                : Clear entire grocery list (static)
*/
struct GroceryItem {
public:
    int id;
    string name;
    string quantity;
    string unit;

    GroceryItem() { id = 0; }                       // default constructor

    GroceryItem(string n, string q, string u) : name(n), quantity(q), unit(u) { id = 0; }  // constructor with name, quantity, unit

    GroceryItem(int i, string n, string q, string u) : id(i), name(n), quantity(q), unit(u) {}  // constructor with all fields

    /*
        Convert to CSV row
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
        Create from CSV row
    */
    static GroceryItem fromCSVRow(vector<string> row) {
        GroceryItem item;
        if (row.size() >= 4) {                      // check valid row format
            int pid = 0;
            try { pid = stoi(row[0]); } catch (...) { pid = 0; }   // parse id from string
            item.id = pid;                          // set id
            item.name = row[1];                     // set name
            item.quantity = row[2];                 // set quantity
            item.unit = row[3];                     // set unit
        }
        return item;                                // return grocery item
    }

    /*
        Load all grocery items
    */
    static vector<GroceryItem> loadAll() {
        vector<GroceryItem> items;                  // create empty vector
        CSV csv("grocery.csv");                     // create csv object
        vector<vector<string>> data = csv.read();   // read csv data
        for (int i = 0; i < data.size(); i++) {
            if (!data[i].empty()) {                 // valid row
                items.push_back(fromCSVRow(data[i]));   // convert and add to items
            }
        }
        return items;                               // return all items
    }

    /*
        Find by id
    */
    static GroceryItem findById(int gid) {
        vector<GroceryItem> all = loadAll();        // load all items
        for (int i = 0; i < all.size(); i++) {
            if (all[i].id == gid) {                 // match found
                return all[i];                      // return item
            }
        }
        return GroceryItem();                       // return empty if not found
    }

    /*
        Find by name and unit (case-insensitive name)
    */
    static GroceryItem findByNameAndUnit(string gname, string gunit) {
        vector<GroceryItem> all = loadAll();        // load all items
        string n1 = out.toLowerCase(out.trim(gname));   // normalize search name
        string u1 = out.trim(gunit);                // normalize search unit
        for (int i = 0; i < all.size(); i++) {
            string n2 = out.toLowerCase(out.trim(all[i].name));     // normalize item name
            string u2 = out.trim(all[i].unit);      // normalize item unit
            if (n1 == n2 && u1 == u2) {             // match found
                return all[i];                      // return item
            }
        }
        return GroceryItem();                       // return empty if not found
    }

    /*
        Save item (merge duplicates by name+unit by adding quantities)
    */
    void save() {
        CSV csv("grocery.csv");                     // create csv object
        vector<vector<string>> data = csv.read();   // read existing data

        GroceryItem existing = findByNameAndUnit(name, unit);   // check for duplicate
        if (existing.id > 0) {                      // duplicate found
            double a = 0.0; double b = 0.0;
            try { a = stod(existing.quantity); } catch (...) { a = 0.0; }   // parse existing quantity
            try { b = stod(quantity); } catch (...) { b = 0.0; }            // parse new quantity
            double t = a + b;                       // add quantities
            string ts = to_string(t);               // convert to string
            for (int i = 0; i < data.size(); i++) {
                if (data[i].size() >= 4) {          // valid row
                    int rid = 0; try { rid = stoi(data[i][0]); } catch (...) { rid = 0; }   // parse row id
                    if (rid == existing.id) {       // found matching row
                        data[i][2] = ts;            // update quantity
                        break;                      // exit loop
                    }
                }
            }
            csv.write(data);                        // write updated data
            out.coutln("Updated grocery item '" + name + "' to " + ts + " " + unit);
            out.br();
            return;                                 // exit method
        }

        int maxId = 0;                              // find max id for auto-increment
        for (int i = 0; i < data.size(); i++) {
            if (data[i].size() >= 1) {              // valid row
                int parsed = 0; try { parsed = stoi(data[i][0]); } catch (...) { parsed = 0; }     // parse id
                if (parsed > maxId) { maxId = parsed; }     // update max id
            }
        }
        id = maxId + 1;                             // set new id
        data.push_back(toCSVRow());                 // add new row
        if (csv.write(data)) {                      // write to file
            out.coutln("Added to grocery list: '" + name + "' (" + quantity + " " + unit + ")");
        } else {
            out.coutln("Error: Could not save grocery item.");
        }
        out.br();
    }

    /*
        Delete by id
    */
    static bool deleteById(int gid) {
        CSV csv("grocery.csv");                     // create csv object
        vector<vector<string>> data = csv.read();   // read existing data
        vector<vector<string>> newData;             // create new data vector
        bool found = false;
        for (int i = 0; i < data.size(); i++) {
            if (data[i].size() >= 4) {              // valid row
                int rid = 0; try { rid = stoi(data[i][0]); } catch (...) { rid = 0; }      // parse row id
                if (rid == gid) { found = true; continue; }     // skip this row (delete it)
            }
            newData.push_back(data[i]);             // add row to new data
        }
        if (found) { csv.write(newData); return true; }     // write updated data
        return false;                               // not found
    }

    /*
        Update quantity by id
    */
    static bool updateQuantityById(int gid, string newq) {
        CSV csv("grocery.csv");                     // create csv object
        vector<vector<string>> data = csv.read();   // read existing data
        bool found = false;
        for (int i = 0; i < data.size(); i++) {
            if (data[i].size() >= 4) {              // valid row
                int rid = 0; try { rid = stoi(data[i][0]); } catch (...) { rid = 0; }      // parse row id
                if (rid == gid) { data[i][2] = newq; found = true; break; }     // update quantity
            }
        }
        if (found) { csv.write(data); return true; }    // write updated data
        return false;                               // not found
    }

    /*
        Clear entire grocery list
    */
    static void clearAll() {
        CSV csv("grocery.csv");                     // create csv object
        vector<vector<string>> empty;               // create empty vector
        csv.write(empty);                           // write empty data (clear file)
    }
};

#endif                                              // GROCERY_H
