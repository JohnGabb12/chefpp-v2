#include "vendor/app.h"
#include <iostream>
#include <vector>
using namespace std;

class MainApp : public App
{
private:
    void header(string toOutput)
    {
        cout << "================" << endl
             << toOutput << endl
             << "================" << endl;
    }

    void mainMenu()
    {
        this->displayChoices({
            "Recipe Manager",
            "Pantry Inventory Manager",
            "Meal Planner",
            "Shopping List Generator",
            "Exit"
        });
        hr();
    }

public:
    const string appName = "Chef++";

    MainApp() : App() {}

    void controller()
    {
        this->mainMenu();
        int choice = inputi("Select an option: ");
        switch (choice)
        {
        case 1:
            coutln("Recipe Manager selected");
            break;
        case 2:
            coutln("Pantry Inventory Manager selected");
            break;
        case 3:
            coutln("Meal Planner selected");
            break;
        case 4:
            coutln("Shopping List Generator selected");
            break;
        case 5:
            coutln("Exiting...");
            break;
        default:
            coutln("Invalid option");
            break;
        }
    }

    // Start
    void start() override
    {
        this->header("Welcome to " + this->appName + "!");
        this->controller();
    }
};
