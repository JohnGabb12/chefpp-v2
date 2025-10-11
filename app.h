#ifndef APP_H // for no dup def
#define APP_H

#include <iostream>
#include <string>

using namespace std;

class App {
private:
    // vars

public:
    // no need for constructor

    void introduction() {
        cout << "Welcome to Chef++" << endl;
    }
    
    void close() {
        cout << endl << "Press enter to stop program...";
        cin.get();
    }

    void start() {
        introduction();

        // main logic
    }

    
};



#endif // APP_H