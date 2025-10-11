#ifndef APP_H // for no dup def
#define APP_H

#include <iostream>
#include <string>

using namespace std;

class App {
private:

public:
    // no need for constructor

    void introduction() {
        cout << "Welcome to Chef++";
    }

    void start() {
        introduction();
        
    }
};



#endif // APP_H