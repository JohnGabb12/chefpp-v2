
#include "../vendor/sys/out.h"
#include "../vendor/base/page.h"
#include "../vendor/sys/options.h"

class IndexPage : public Page {
protected:
    string version = "1.0";

    void logo() {
        out.coutln(out.center(" _______  __   __  _______  _______    _       _    "));
        out.coutln(out.center("|       ||  | |  ||       ||       | _| |_   _| |_  "));
        out.coutln(out.center("|       ||  |_|  ||    ___||    ___||_   _| |_   _| "));
        out.coutln(out.center("|       ||       ||   |___ |   |___   |_|     |_|   "));
        out.coutln(out.center("|      _||       ||    ___||    ___|                "));
        out.coutln(out.center("|     |_ |   _   ||   |___ |   |                    "));
        out.coutln(out.center("|_______||__| |__||_______||___|                    "));
    }

    void schema() override {
        logo();
        out.br();
        out.coutln(out.center("Version: " + version));
        out.br();
    }

public:
    IndexPage() : Page() {
        // Set to show "Exit" instead of "Back"
        useExitInsteadOfBack = true;
        showheader = false;
        
        // Initialize the parent class options vector
        options = {
            Options("Recipe Manager", []() {
                out.coutln("Recipe Manager selected");
            }),
            Options("Ingredient Manager", []() {
                out.coutln("Ingredient Manager selected");
            })
        };
    }
};