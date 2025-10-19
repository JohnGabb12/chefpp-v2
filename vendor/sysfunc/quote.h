#ifndef QUOTE_H
#define QUOTE_H

#include "out.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>

using namespace std;

class Quote {
private:
    vector<string> quotes;
    const string filename = "data/quotes.json";
    bool loaded = false;

    void loadQuotes() {
        ifstream file(filename);
        if (!file.is_open()) {
            quotes.push_back("Cook with love and passion!");
            loaded = true;
            return;
        }
        
        string line, content;
        while (getline(file, line)) {
            content += line;
        }
        file.close();

        // Simple manual parsing to avoid any potential issues
        size_t quotesStart = content.find("\"quotes\"");
        if (quotesStart == string::npos) {
            quotes.push_back("Cook with love and passion!");
            loaded = true;
            return;
        }
        
        size_t arrayStart = content.find("[", quotesStart);
        size_t arrayEnd = content.find("]", arrayStart);
        if (arrayStart == string::npos || arrayEnd == string::npos) {
            quotes.push_back("Cook with love and passion!");
            loaded = true;
            return;
        }
        
        // Extract each quote string
        size_t pos = arrayStart + 1;
        while (pos < arrayEnd) {
            // Skip whitespace and commas
            while (pos < arrayEnd && (content[pos] == ' ' || content[pos] == '\n' || 
                   content[pos] == '\r' || content[pos] == '\t' || content[pos] == ',')) {
                pos++;
            }
            
            if (pos >= arrayEnd) break;
            
            // Found a quote
            if (content[pos] == '"') {
                pos++; // skip opening quote
                size_t quoteEnd = pos;
                while (quoteEnd < arrayEnd && content[quoteEnd] != '"') {
                    if (content[quoteEnd] == '\\') quoteEnd++; // skip escaped char
                    quoteEnd++;
                }
                
                if (quoteEnd < arrayEnd) {
                    quotes.push_back(content.substr(pos, quoteEnd - pos));
                    pos = quoteEnd + 1; // skip closing quote
                }
            } else {
                pos++;
            }
        }
        
        if (quotes.empty()) {
            quotes.push_back("Cook with love and passion!");
        }
        loaded = true;
    }

public:
    Quote() = default;

    string getRandomQuote() {
        if (!loaded) {
            loadQuotes();
        }
        if (quotes.empty()) return "Cook with love and passion!";
        static mt19937 rng(static_cast<unsigned int>(time(nullptr)));
        uniform_int_distribution<size_t> dist(0, quotes.size() - 1);
        return quotes[dist(rng)];
    }

    static Quote& getInstance() {
        static Quote instance;
        return instance;
    }
};

#endif // QUOTE_H
