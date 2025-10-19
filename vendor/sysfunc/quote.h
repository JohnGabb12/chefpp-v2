#ifndef QUOTE_H
#define QUOTE_H

#include "../vendor/sysfunc/out.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>

using namespace std;

class Quote
{
private:
    vector<string> quotes;
    const string filename = "data/quotes.json";

    void loadQuotes()
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            quotes.push_back("Cook with love and passion!");
            return;
        }

        stringstream buffer;
        buffer << file.rdbuf();
        string content = buffer.str();
        file.close();

        quotes = out.extractJsonArray(content, "quotes");

        if (quotes.empty())
        {
            quotes.push_back("Cook with love and passion!");
        }
    }

public:
    Quote()
    {
        srand(static_cast<unsigned int>(time(0)));
        loadQuotes();
    }

    string getRandomQuote()
    {
        if (quotes.empty())
            return "Cook with love and passion!";

        int index = rand() % quotes.size();
        return quotes[index];
    }

    static Quote& getInstance()
    {
        static Quote instance;
        return instance;
    }
};

#endif // QUOTE_H
