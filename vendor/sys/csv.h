#ifndef CSV_H // for no dup def
#define CSV_H

#include "../sys/out.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "../database/seeder.h"

using namespace std;

/*
    CSV Class

    This class provides methods to read from and write to CSV (Comma-Separated Values) files.
    It handles basic CSV formatting, including quoting fields with commas or quotes.

    How it works:
        - The constructor takes a filename and ensures the file exists.
        - The read() method reads the entire CSV file into a 2D vector of strings.
        - The write() method writes a 2D vector of strings to the CSV file.
        - Private helper methods handle parsing lines, writing rows, and escaping quotes.

    Header classes:
    #include "../sys/out.h"
    #include <string>
    #include <vector>
    #include <iostream>
    #include <fstream>
    #include <sstream>
    #include <algorithm>

    CSV:
        private:
            - filename                              : The name of the CSV file.
            - needsQuoting(string)                  : Checks if a field needs to be quoted.
            - escapeQuotes(string)                  : Escapes quotes in a field.
            - trim(string)                          : Trims whitespace from a string.
            - parseLine(string)                     : Parses a CSV line into fields.
            - writeRow(ofstream&, vector<string>)   : Writes a single row to the CSV file.
            - ensureFileExists()                    : Creates the file if it doesn't exist.
        
        public:
            - CSV(string)                           : Constructor that takes a filename.
            - read()                                : Reads the entire CSV file into a 2D vector.
            - write(vector<vector<string>>)         : Writes a 2D vector to the CSV file.
*/
class CSV {
private:
    string filename;

    /*
        Checks if a field needs to be quoted in the CSV output.
    */
    bool needsQuoting(string field) {
        return field.find(',') != string::npos ||                           // check if field contains comma
               field.find('"') != string::npos ||                           // check if field contains quotes
               field.find('\n') != string::npos ||                          // check if field contains newline
               field.find('\r') != string::npos;                            // check if field contains carriage return
    }

    /*
        Escapes quotes in a field by doubling them.
    */
    string escapeQuotes(string field) {
        string result;                                                      // initialize result string
        for (char c : field) {                                              // loop through each character
            if (c == '"') result += "\"\"";                                 // double the quotes for CSV escape
            else result += c;                                               // add character as is
        }
        return result;                                                      // return escaped string
    }

    /*
        Trims whitespace from the beginning and end of a string.
    */
    string trim(string str) {
        int first = str.find_first_not_of(" \t\r\n");                       // trims the left side
        if (first == string::npos) return "";                               // return empty if all whitespace
        int last = str.find_last_not_of(" \t\r\n");                         // trims the right side
        return str.substr(first, (last - first + 1));                       // return trimmed string
    }

    /*
        Parses a single CSV line into fields, handling quoted fields.
    */
    vector<string> parseLine(string line) {
        vector<string> result;                                              // store parsed fields
        string field;                                                       // current field being built
        bool inQuotes = false;                                              // track if we're inside quotes

        for (int i = 0; i < line.length(); i++) {                           // loop through each character
            char c = line[i];                                               // get current character
            if (c == '"') {                                                 // found a quote character
                inQuotes = !inQuotes;                                       // toggle quote state
            } else if (c == ',' && !inQuotes) {                             // found delimiter outside quotes
                result.push_back(field);                                    // save current field
                field.clear();                                              // reset for next field
            } else {
                field += c;                                                 // add to current field
            }
        }

        result.push_back(trim(field));                                      // add last field with trimming
        return result;                                                      // return all parsed fields
    }

    /*
        Writes a single row to the CSV file.
    */
    void writeRow(ofstream& file, vector<string> row) {
        for (int i = 0; i < row.size(); i++) {                              // loop through each field in row
            if (i > 0) file << ",";                                         // add comma before field (except first)

            // Check if field needs quoting
            if (needsQuoting(row[i])) {                                     // field contains special characters
                file << "\"" << escapeQuotes(row[i]) << "\"";               // write field with quotes and escaping
            } else {                                                        // field is simple
                file << row[i];                                             // write field as is
            }
        }
        file << "\n";                                                       // end line with newline
    }

    /*
        Creates the directory if it doesn't exist.
    */
    void ensureDirectoryExists() {
        #if defined(_WIN32) || defined(_WIN64)                              // Windows OS
            system("if not exist .\\data mkdir .\\data");                   // create data directory if missing
        #else                                                               // Unix/Linux/Mac OS
            system("mkdir -p ./data");                                      // create data directory if missing
        #endif
    }

    /*
        Creates the file if it doesn't exist.
    */
    void ensureFileExists() {
        ensureDirectoryExists();                                            // ensure data directory exists first
        
        ifstream checkFile(filename);                                       // try to open file for reading
        if (!checkFile.good()) {                                            // file doesn't exist
            // File doesn't exist, create it
            ofstream createFile(filename);                                  // create new file
            if (createFile.is_open()) {
                createFile.close();                                         // close the file
                out.coutln("Created new CSV file: " + filename);            // notify user
                // If the created file is the recipes CSV, seed it now.
                try {
                    string tail = filename;
                    for (int i = 0; i < (int)tail.length(); ++i) if (tail[i] == '\\') tail[i] = '/';
                    if (tail.size() >= 11 && tail.substr(tail.size() - 11) == "recipes.csv") {
                        Seeder::seedRecipes(filename, 100);
                    }
                } catch (...) {
                    // ignore seeder errors
                }
            } else {
                out.coutln("Error: Could not create file " + filename);     // notify error
            }
        } else {                                                            // file already exists
            checkFile.close();                                              // close the file
        }
    }

public:
    /*
        Constructor
            - file: CSV file name
    */
    CSV(string file) : filename("./data/" + file) {                        // set filename with data/ prefix
        ensureFileExists();                                                 // create file if it doesn't exist
    }

    /*
        Read entire CSV file into a 2D vector
    */
    vector<vector<string>> read() {
        vector<vector<string>> data;                                        // store all rows of data
        
        // Ensure file exists before reading
        ensureFileExists();                                                 // create file if missing
        
        ifstream file(filename);                                            // open file for reading

        if (!file.is_open()) {                                              // failed to open file
            out.coutln("Error: Could not open file " + filename);           // notify error
            return data;                                                    // return empty data
        }

        string line;                                                        // store each line
        while (getline(file, line)) {                                       // read file line by line
            data.push_back(parseLine(line));                                // parse and add to data
        }
        
        file.close();                                                       // close the file
        return data;                                                        // return all parsed data
    }

    /*
        Write a 2D vector to the CSV file
    */
    bool write(vector<vector<string>> data) {
        // Ensure file exists before writing
        ensureFileExists();                                                 // create file if missing
        
        ofstream file(filename);                                            // open file for writing

        if (!file.is_open()) {                                              // failed to open file
            out.coutln("Error: Could not open file " + filename);           // notify error
            return false;                                                   // return failure
        }

        for (auto row : data) {                                             // loop through each row
            writeRow(file, row);                                            // write row to file
        }

        file.close();                                                       // close the file
        return true;                                                        // return success
    }

};

#endif // CSV_H