#ifndef APP_H // for no dup def
#define APP_H
#include <iostream>
#include <string>
#include <vector>
/*

App class for controlling the application

This class contains all the logic for the application, including the main loop and user interaction.

*/
class App {
public:
    virtual void start() {
        std::cout << "Hello world!" << std::endl;
    }
    virtual void close() {
        std::cout << "Press any key to exit..." << std::endl;
        std::cin.get();
    }
};


#endif // APP_H