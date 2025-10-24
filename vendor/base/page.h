#ifndef PAGE_H // for no dup def
#define PAGE_H

#include "../sys/out.h"
#include "../sys/options.h"

using namespace std;

/*
    Page Class

    This class represents a page in the application.
    It provides methods to display the page, handle user input, and navigate between pages.
    It serves as a base class for all pages in the application, providing a consistent interface and behavior.

    How it works:
        - Each page has a header, options, and a schema method to define its contents.
        - The init method initializes the page, displays the header, schema, options, and handles user input.
        - The displayOptions method displays all available options on the page.
        - The handleInput method processes user input and executes the corresponding option function.
            - Each option is represented by an Options object, which contains a name and a function to execute.
            - The option func() can contain another page class which can be used to navigate to another page.
        - The schema method is a pure virtual function that must be implemented by derived classes to define the page's contents.

    Header classes:
    #include "../vendor/sys/out.h"
    #include "../vendor/sys/options.h"

    Page:
        private:
            - displayBackOption()   : Displays the back option for navigation.
        protected:
            - header                : The header text for the page.
            - showheader            : Boolean to control header display.
            - options               : Vector of Options objects representing available options.
            - displayOptions()      : Displays all options on the page.
            - handleInput()         : Handles user input and executes corresponding option functions.
            - schema()              : Pure virtual function to define page contents.
        public:
            - Page()                : Default constructor.
            - init()                : Initializes the page, displays header, schema, options, and handles input.

*/
class Page {
private:
    /*
        Display back option
    */
    void displayBackOption() {
        int optionscount = options.size() + 1;          // +1 for back option
        string label = useExitInsteadOfBack ? "Exit" : "Back";  // choose label
        out.coutln(to_string(optionscount) + ". " + label);     // display option
    }

protected:
    /*
        Protected variables
    */
    string header = "";
    bool showheader = true;
    bool useExitInsteadOfBack = false;              // set to true to show "Exit" instead of "Back"

    /*
        Display all options on the page
    */
    void displayOptions() {
        for (int i = 0; i < options.size(); i++) {
            int optionscount = i + 1;                   // +1 for display index
            out.coutln(to_string(optionscount) + ". " + options[i].name);
        }
        displayBackOption();
    }

    /*
        Handle user input for options
    */
    string handleInput() {
        int choice = out.inputi("Select an option: ");  // prompt for input
        if (choice == options.size() + 1) {             // back option
            return "<back>";                            // go back
        }
        if (choice > 0 && choice <= options.size()) {   // options
            options[choice - 1].func();                 // execute option function
        } else {
            out.coutln("Invalid choice");               // invalid choice
        }
        return "<continue>";                            // continue
    }

    /*
        Abstract schema method
            - defines page contents
    */
    virtual void schema() = 0;

public:

    /*
        Constructor
            - No need because default are already set by inherited classes
    */
    Page() {}
    
    /*
        Initialize page
            - clear ui
            - display header if enabled
            - display page contents
            - display horizontal rule
            - display options
            - handle user input
    */
    void init() {
        out.clear();                                // clear ui
        if (this->showheader) {
            out.subheader(this->header);            // display header
        }
        schema();                                   // display page contents
        out.hr();                                   // horizontal rule
        displayOptions();                           // display options
        out.br();
        string result = handleInput();              // handle user input
        if (result == "<back>") {
            return;                                 // go back
        }
        this->init();                               // re-init to redraw after action returns
    }

    vector<Options> options;
};

#endif // PAGE_H