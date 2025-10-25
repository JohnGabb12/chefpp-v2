#ifndef MEALPLAN_H
#define MEALPLAN_H

#include "../../vendor/sys/out.h"
#include "../../vendor/sys/csv.h"
#include "../recipemanager/recipe.cpp"
#include <vector>
#include <string>

using namespace std;

/*
    MealPlan Struct

    Represents a single planned meal for a specific week and day.
    Stores the selected recipe id and name for easy display.

    How it works:
        - Saved in CSV file mealplan.csv
        - Provides helpers to save, load, find by week, and clear week

    Header classes:
    #include "../../vendor/sys/out.h"
    #include "../../vendor/sys/csv.h"
    #include "../recipemanager/recipe.cpp"

    MealPlan:
        public:
            - id                 : Auto-increment primary key
            - week               : Week label (e.g., 2025-W43 or any string)
            - day                : Day label (Mon, Tue, ...)
            - recipeId           : Recipe ID from recipes.csv
            - recipeName         : Cached recipe name for quick viewing
            - toCSVRow()         : Convert to CSV row
            - fromCSVRow(row)    : Build from CSV row
            - save()             : Insert or replace entry for same week+day
            - loadAll()          : Load all entries
            - findByWeek(week)   : Load all entries for a given week
            - clearWeek(week)    : Remove all entries for a given week
*/
struct MealPlan {
public:
    int id;
    string week;
    string day;
    int recipeId;
    string recipeName;

    MealPlan() {
        id = 0; week = ""; day = ""; recipeId = 0; recipeName = "";
    }

    MealPlan(string w, string d, int rid, string rname) {
        id = 0; week = w; day = d; recipeId = rid; recipeName = rname;
    }

    vector<string> toCSVRow() {
        vector<string> row;
        row.push_back(to_string(id));
        row.push_back(week);
        row.push_back(day);
        row.push_back(to_string(recipeId));
        row.push_back(recipeName);
        return row;
    }

    static MealPlan fromCSVRow(vector<string> row) {
        MealPlan m;
        if (row.size() >= 5) {
            int parsedId = 0; int parsedRid = 0;
            try { parsedId = stoi(row[0]); } catch (...) { parsedId = 0; }
            try { parsedRid = stoi(row[3]); } catch (...) { parsedRid = 0; }
            m.id = parsedId;
            m.week = row[1];
            m.day = row[2];
            m.recipeId = parsedRid;
            m.recipeName = row[4];
        } else if (row.size() >= 4) {
            int parsedId2 = 0; int parsedRid2 = 0;
            try { parsedId2 = stoi(row[0]); } catch (...) { parsedId2 = 0; }
            try { parsedRid2 = stoi(row[3]); } catch (...) { parsedRid2 = 0; }
            m.id = parsedId2;
            m.week = row[1];
            m.day = row[2];
            m.recipeId = parsedRid2;
            m.recipeName = "";
        }
        return m;
    }

    void save() {
        CSV csv("mealplan.csv");
        vector<vector<string>> data = csv.read();

        // determine next id
        int maxId = 0;
        for (int i = 0; i < data.size(); i++) {
            if (data[i].size() >= 1) {
                int val = 0; try { val = stoi(data[i][0]); } catch (...) { val = 0; }
                if (val > maxId) { maxId = val; }
            }
        }

        // replace existing entry for same week + day
        vector<vector<string>> newData;
        bool replaced = false;
        for (int i = 0; i < data.size(); i++) {
            if (data[i].size() >= 5) {
                string w = data[i][1];
                string d = data[i][2];
                if (w == week && d == day) {
                    if (id == 0) {
                        int pid = 0;
                        if (data[i][0] != "") {
                            try { pid = stoi(data[i][0]); } catch (...) { pid = 0; }
                        }
                        id = pid;
                    }
                    vector<string> row = toCSVRow();
                    newData.push_back(row);
                    replaced = true;
                    continue;
                }
            }
            newData.push_back(data[i]);
        }

        if (!replaced) {
            id = maxId + 1;
            newData.push_back(toCSVRow());
        }

        bool ok = csv.write(newData);
        if (ok) {
            out.coutln("Saved meal plan for " + week + " - " + day + ": " + recipeName);
        } else {
            out.coutln("Error: Could not save meal plan.");
        }
        out.br();
    }

    static vector<MealPlan> loadAll() {
        vector<MealPlan> items;
        CSV csv("mealplan.csv");
        vector<vector<string>> data = csv.read();
        for (int i = 0; i < data.size(); i++) {
            if (data[i].size() >= 4) {
                items.push_back(fromCSVRow(data[i]));
            }
        }
        return items;
    }

    static vector<MealPlan> findByWeek(string w) {
        vector<MealPlan> all = loadAll();
        vector<MealPlan> result;
        for (int i = 0; i < all.size(); i++) {
            if (all[i].week == w) { result.push_back(all[i]); }
        }
        return result;
    }

    static void clearWeek(string w) {
        CSV csv("mealplan.csv");
        vector<vector<string>> data = csv.read();
        vector<vector<string>> filtered;
        for (int i = 0; i < data.size(); i++) {
            if (data[i].size() >= 3) {
                if (data[i][1] == w) { continue; }
            }
            filtered.push_back(data[i]);
        }
        csv.write(filtered);
    }
};

#endif // MEALPLAN_H
