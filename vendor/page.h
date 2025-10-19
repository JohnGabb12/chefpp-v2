#ifndef PAGE_H // for no dup def
#define PAGE_H
#include <cstdlib>
#include <string>
#include "../vendor/sysfunc/out.h"
using namespace std;

class Page {
protected:
    vector<string> parentNames;
    // render breadcrumbs for nested pages
    void renderBreadcrumbs() const {
        string path = "Home";
        for (const auto& p : parentNames) {
            path += " > " + p;
        }
        path += " > " + name;
        out.coutln(path);
    }

public:
    string name = "";

    Page() {}

    Page(string name, vector<string> parents = {}) : name(name), parentNames(parents) {}

    virtual void schema() = 0; // contents

    void init() {
        out.clear();
        renderBreadcrumbs();
        out.br(); out.br();
        schema();
    }
};

#endif // PAGE_H