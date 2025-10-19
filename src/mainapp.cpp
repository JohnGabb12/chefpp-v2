#include "../vendor/app.h"
#include "../vendor/page.h"
#include "../vendor/sysfunc/out.h"
#include "../vendor/sysfunc/quote.h"
#include "RecipeManager.cpp"
#include <iostream>
using namespace std;

class MainApp : public App, public Page
{
private:
    // pages would go here
    RecipeManager recipeManager;
    Quote quote;
    
    const string version = "1.0";

public:
    const string appName = "Chef++";

    MainApp() : App(), Page("Main", "Main Menu", {}, /*showBreadcrumbs=*/false, /*showSubheader=*/false)
    {
    }

    void schema() override
    {
        out.header("Welcome to " + appName + "! " + "(v" + version + ")");
        out.coutln(quote.getRandomQuote());
        out.coutln("Your personal chef assistant.");
        out.br();
        out.displayChoices({"Recipe Manager", "Ingredient Manager", "Meal Planner", "Shopping List", "Exit"});
        out.br();
        int choice = out.inputi("Select an option: ");
        switch (choice)
        {
        case 1:
            out.coutln("Recipe Manager selected");
            out.br();
            recipeManager.init();
            break;
        case 2:
            out.coutln("Ingredient Manager selected");
            break;
        case 3:
            out.coutln("Meal Planner selected");
            break;
        case 4:
            out.coutln("Shopping List selected");
            break;
        case 5:
            out.coutln("Exiting...");
            exit(0);
            break;
        default:
            out.coutln("Invalid choice");
            break;
        }
        // Re-init to redraw after action returns
        this->init();
    }

    // Start
    void start() override
    {
        this->init();
    }
};
