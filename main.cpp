#include <iostream>
#include "app.h"

using namespace std;

void appclose() {
    cout << endl << "Press enter to stop program...";
    cin.get();
}

int main() {
    App app; 
    
    app.start(); // seperate controller for app

    appclose();
    return 0;
}