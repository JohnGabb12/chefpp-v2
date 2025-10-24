#ifndef OPTIONS_H // for no dup def
#define OPTIONS_H

#include <vector>
#include <string>
#include <functional>

using namespace std;

/*
    Options Struct

    This struct represents an option in the application.
    It contains the name of the option and a function to execute when the option is selected.

    How it works:
        - Each option has a name and a function.
        - The name is displayed to the user.
        - The func() method is called when the option is selected by the user.

    Options:
        - name                  : The name of the option.
        - Options(string name)  : Initializes the option with a name.
        - func()                : The function to execute when the option is selected.

*/
struct Options {
    /* 
        Option name 
    */
    string name;
    
    /*
        Function callback
    */
    function<void()> callback;

    /*
        Constructor
    */
    Options(string name, function<void()> fn) : name(name), callback(fn) {};

    /*
        Function to execute when option is selected
    */
    void func() {
        callback();
    }

    virtual ~Options() {}
};


#endif // OPTIONS_H