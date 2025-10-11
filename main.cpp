#include <iostream>
#include "app.h"

using namespace std;

int main() {
    App app;

    app.start(); // separate controller for app

    app.close();
    return 0;
}