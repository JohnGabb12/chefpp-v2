#include "../vendor/page.h"
#include "../vendor/sysfunc/out.h"
#include <iostream>
using namespace std;

class RecipeManager : public Page {
public:
    RecipeManager() : Page("Recipe Manager", {}) {}

    void schema() override {
        cout << "Welcome to the " << name << endl;
        // Display recipe management options here
    }
};
