#ifndef MODAL_H // for no dup def
#define MODAL_H

#include "../sys/out.h"
#include <iostream>

using namespace std;

/*
    Modal Class

    This class represents a modal dialog in the application.
    It provides methods to display the modal contents and handle user interaction.

    How it works:
        - Each modal has a schema method to define its contents.
        - The init method initializes the modal, displays the contents, and waits for user input to exit.
        - The displayExitPrompt method displays a prompt for the user to press Enter to exit the modal.
        - The schema method is a pure virtual function that must be implemented by derived classes to define the modal's contents.

    Header classes:
    #include "../vendor/sys/out.h"
    #include <iostream>

    Modal:
        private:
            - displayExitPrompt()   : Displays a prompt for the user to press Enter to exit the modal.
        protected:
            - schema()              : Pure virtual function to define modal contents.
        public:
            - Modal()               : Default constructor.
            - init()                : Initializes the modal, displays contents, and waits for user input to exit.

*/
class Modal {
private:
    /*
        Display exit prompt
    */
    void displayExitPrompt() {
        out.coutln("Press Enter to exit...");       // display exit prompt
        cin.get();                                  // wait for user input
    }

protected:
    /*
        Abstract schema method
            - defines modal contents
    */
    virtual void schema() = 0;
    
public:

    /*
        Constructor
            - No need because default are already set by inherited classes
    */
    Modal() {}

    /*
        Initialize modal
            - clear ui
            - display modal contents
            - display exit prompt
    */
    void init() {
        out.clear();                                // clear ui
        schema();                                   // display modal contents
        out.br();
        displayExitPrompt();                        // display exit prompt
    }
};


#endif // MODAL_H