#include "vendor/app.h"
#include <iostream>
using namespace std;

class MainApp : public App {
public:

    const string appName = "ChefPP";

    void introduction() {
        cout << "Welcome to " << this->appName << "!" << endl;
    }


    // Start
    void start() {
        introduction();
    }

    // Close
    void close() {

    }
};
