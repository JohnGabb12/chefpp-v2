#ifndef PAGE_H // for no dup def
#define PAGE_H
#include <cstdlib>
#include <string>
#include <vector>
#include "../vendor/sysfunc/out.h"
using namespace std;

class Page {
protected:
    vector<string> parentNames;
    bool showBreadcrumbs = true;
    bool showSubheader = true;
    // render breadcrumbs for nested pages
    void renderBreadcrumbs() const {
        string path = "Home";
        for (const auto& p : parentNames) {
            path += " > " + p;
        }
        path += " > " + name;
        out.coutln(path);
    }

    string name = "";
    string label = "";

public:

    Page() {}

    Page(string name, string label, vector<string> parents = {}, bool showBreadcrumbs = true, bool showSubheader = true)
        : name(name), label(label), parentNames(parents), showBreadcrumbs(showBreadcrumbs), showSubheader(showSubheader) {}

    virtual void schema() = 0; // contents

    void init() {
        out.clear();
        if (showBreadcrumbs) {
            renderBreadcrumbs();
            out.br();
            out.br();
        }
        if (showSubheader) {
            out.subheader(this->label);
        }
        schema();
    }

    // Runtime controls
    void setShowBreadcrumbs(bool show) { showBreadcrumbs = show; }
    void setShowSubheader(bool show) { showSubheader = show; }
    void setLabel(const string& newLabel) { label = newLabel; }
    void setName(const string& newName) { name = newName; }
    void setParents(const vector<string>& parents) { parentNames = parents; }
    void pushParent(const string& parent) { parentNames.push_back(parent); }
    void popParent() { if (!parentNames.empty()) parentNames.pop_back(); }
};

#endif // PAGE_H