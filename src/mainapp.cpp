#include "../vendor/app.h"
#include "../vendor/sysfunc/out.h"
#include "RecipeManager.cpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <cstdlib>
using namespace std;

class MainApp : public App
{
private:

    // pages would go here
    RecipeManager recipeManager;

    const string version = "1.0";
    vector<string> quotes;

    // Load quotes from JSON file
    void loadQuotes() {
        ifstream file("data/quotes.json");
        if (!file.is_open()) {
            quotes.push_back("Cooking is an art, and the kitchen is your canvas.");
            return;
        }

        string line, content;
        while (getline(file, line)) {
            content += line;
        }
        file.close();

        // Simple JSON parsing for the quotes array
        size_t start = content.find("[");
        size_t end = content.find("]");
        if (start == string::npos || end == string::npos) {
            quotes.push_back("Cooking is an art, and the kitchen is your canvas.");
            return;
        }

        string quotesSection = content.substr(start + 1, end - start - 1);
        size_t pos = 0;
        while ((pos = quotesSection.find("\"", pos)) != string::npos) {
            size_t quoteStart = pos + 1;
            size_t quoteEnd = quotesSection.find("\"", quoteStart);
            if (quoteEnd == string::npos) break;
            
            string quote = quotesSection.substr(quoteStart, quoteEnd - quoteStart);
            if (!quote.empty() && quote != "quotes") {
                quotes.push_back(quote);
            }
            pos = quoteEnd + 1;
        }

        if (quotes.empty()) {
            quotes.push_back("Cooking is an art, and the kitchen is your canvas.");
        }
    }

    string getRandomQuote() {
        if (quotes.empty()) return "";
        return quotes[rand() % quotes.size()];
    }

public:
    const string appName = "Chef++";

    MainApp() : App() {
        srand(static_cast<unsigned int>(time(0))); // Seed random number generator
        loadQuotes();
    }

    void controller()
    {
        // menu page here
        out.displayChoices({"Recipe Manager", "Ingredient Manager", "Meal Planner", "Shopping List", "Exit"});
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
    }

    // Start
    void start() override
    {
        // loop
        while (true) {
            out.header("Welcome to " + appName + "! " + "(v" + version + ")");
            out.coutln(getRandomQuote());
            out.coutln("Your personal chef assistant.");
            out.br();
            out.coutln("Main Menu:");
            this->controller();
            out.clear();
        }
    }
};
