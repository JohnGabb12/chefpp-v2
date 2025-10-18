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

    void coutln(const std::string& toOutput) {
        std::cout << toOutput << std::endl;
    }

    int inputi(const std::string& prompt) {
        int input;
        std::cout << prompt;
        std::cin >> input;
        std::cin.ignore(); // clear newline
        return input;
    }

    void hr() {
        std::cout << "----------------" << std::endl;
    }

    void displayChoices(const std::vector<std::string>& choices) {
        for (size_t i = 0; i < choices.size(); ++i) {
            std::cout << (i + 1) << ". " << choices[i] << std::endl;
        }
    }
};


#endif // APP_H