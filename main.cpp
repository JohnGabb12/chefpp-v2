#include <iostream>
#include "src/index.cpp"


using namespace std;

/*

Main entry point for the application

Do not put any logic here nor mess with the contents.
All logic should be in app.h

*/

// Global width variable for console output formatting
int width = 80;

int main() {
    IndexPage index;
    index.init();
    return 0;
}