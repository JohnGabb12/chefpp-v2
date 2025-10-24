#ifndef OUT_H
#define OUT_H

#include <iostream>
#include <string>

using namespace std;

// Forward declaration - width must be defined in main.cpp
extern int width;

/*
    Out Struct

    This struct provides methods for console output and input handling.
    It includes methods for printing lines, reading integer input, clearing the console,
    centering text, displaying headers and subheaders, and printing line breaks and horizontal rules.

    How it works:
        - The coutln() method prints a string followed by a newline.
        - The inputi() method prompts the user for integer input and validates it.
        - The clear() method clears the console screen.
        - The center() method centers a string based on the application width.
        - The header() and subheader() methods display formatted headers.
        - The br() method prints a blank line.
        - The hr() method prints a horizontal rule.

    Header classes:
    #include "../app.h"
    #include <iostream>
    #include <string>

    Out:
        - coutln(string)         : Prints a string followed by a newline.
        - inputi(string)         : Prompts for and returns an integer input.
        - clear()                : Clears the console screen.
        - center(string)         : Centers a string based on application width.
        - header(string)         : Displays a formatted header.
        - subheader(string)      : Displays a formatted subheader.
        - br()                   : Prints a blank line.
        - hr()                   : Prints a horizontal rule.
*/
struct out {

    /* 
        Print line with newline 
    */
    void coutln(string toOutput) {
        cout << toOutput << endl;                                           // output string with newline
    }
    void coutln(int toOutput) {
        cout << toOutput << endl;                                           // output string with newline
    }

    /* 
        Input integer with prompt and validation 
    */
    int inputi(string prompt) {
        string input;                                                       // store user input as string
        while (true) {                                                      // loop until valid input
            if (!prompt.empty()) {                                          // check if prompt is provided
                cout << prompt;                                             // display prompt to user
            }
            getline(cin, input);                                            // read entire line of input

            if (input.empty()) {                                            // check if input is empty
                cout << "Error: Please enter a number." << endl;            // show error message
                this->br();                                                 // add blank line
                continue;                                                   // restart loop
            }

            bool isValid = true;                                            // flag for valid input
            for (char c : input) {                                          // loop through each character
                if (!isdigit(c)) {                                          // check if character is not a digit
                    this->coutln("Error: Invalid input.");                  // show error message
                    this->br();                                             // add blank line
                    isValid = false;                                        // mark as invalid
                    break;                                                  // exit inner loop
                }
            }
            
            if (!isValid) {                                                 // if input was invalid
                continue;                                                   // restart outer loop
            }

            try {                                                           // attempt to convert string to int
                return stoi(input);                                         // convert and return integer
            } catch (out_of_range) {                                        // number is too large
                this->coutln("Error: Number too large.");                   // show error message
            } catch (invalid_argument) {                                    // invalid conversion
                this->coutln("Error: Invalid inpute.");                      // show error message
            }
            this->br();                                                     // add blank line before retry
        }
    }

    /* 
        Input string with prompt 
    */
    string inputs(string prompt) {
        if (!prompt.empty()) {                                              // check if prompt is provided
            cout << prompt;                                                 // display prompt to user
        }
        string input;                                                       // store user input
        getline(cin, input);                                                // read entire line of input
        return input;                                                       // return input string
    }

    /* 
        Input yes/no with prompt and validation 
    */
    bool inputYesNo(string prompt) {
        while (true) {                                                      // loop until valid input
            if (!prompt.empty()) {                                          // check if prompt is provided
                cout << prompt;                                             // display prompt to user
            }
            string input;                                                   // store user input
            getline(cin, input);                                            // read entire line of input
            
            if (!input.empty()) {                                           // check if input is not empty
                char c = tolower(input[0]);                                 // get first character as lowercase
                if (c == 'y') return true;                                  // return true for yes
                if (c == 'n') return false;                                 // return false for no
            }
            this->coutln("Please enter 'y' or 'n'.");                       // show error message
            this->br();                                                     // add blank line
        }
    }

    /* 
        Trim whitespace from beginning and end of string
    */
    string trim(string str) {
        size_t start = str.find_first_not_of(" \t\r\n");                    // find first non-whitespace
        if (start == string::npos) return "";                               // return empty if all whitespace
        size_t end = str.find_last_not_of(" \t\r\n");                       // find last non-whitespace
        return str.substr(start, end - start + 1);                          // return trimmed string
    }

    /* 
        Clear console screen
    */
    void clear() {
        #if defined(_WIN32) || defined(_WIN64)                              // check if Windows OS
            system("cls");                                                  // use Windows clear command
        #else                                                               // Unix/Linux/Mac OS
            system("clear");                                                // use Unix clear command
        #endif
    }

    /* 
        Center string based on App width 
    */
    string center(string toOutput) {
        int len = static_cast<int>(toOutput.length());                      // get length of string
        int w = width;                                                 // get application width
        if (len >= w) {                                                     // string is too long to center
            return toOutput;                                                // return original string
        }
        int padding = (w - len) / 2;                                        // calculate left padding needed
        return string(padding, ' ') + toOutput;                             // return padded string
    }

    /* 
        Display formatted header with equal signs 
    */
    void header(string toOutput) {
        int w = width;                                                 // get application width
        this->coutln(string(w,'='));                                        // print top border
        this->coutln(this->center(toOutput));                               // print centered text
        this->coutln(string(w,'='));                                        // print bottom border
        this->br();                                                         // add blank line after
    }

    /* 
        Display formatted subheader with dashes 
    */
    void subheader(string toOutput) {
        int w = width;                                                 // get application width
        this->coutln(string(w,'-'));                                        // print top border
        this->coutln(this->center(toOutput));                               // print centered text
        this->br();                                                         // add blank line after
    }

    /* 
        Print blank line 
    */
    void br() {
        cout << endl;                                                       // output newline
    }

    /* 
        Print horizontal rule (line of dashes) 
    */
    void hr() {
        int w = width;                                                 // get application width
        this->coutln(string(w,'-'));                                        // print horizontal line
    }
} out;  // Create global instance named 'out'

#endif // OUT_H