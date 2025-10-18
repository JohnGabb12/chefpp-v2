#include <iostream>
#include "src/mainapp.cpp"


using namespace std;

/*

Main entry point for the application

Do not put any logic here nor mess with the contents.
All logic should be in app.h

*/

int main() {
    MainApp app;
    app.start();
    app.close();
    return 0;
}