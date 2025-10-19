#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cctype>
#include <cstdlib>

struct Out {
    void coutln(const std::string& toOutput) const {
        std::cout << toOutput << std::endl;
    }

    void hr() const {
        std::cout << "----------------" << std::endl;
    }

    void displayChoices(const std::vector<std::string>& choices) const {
        for (size_t i = 0; i < choices.size(); ++i) {
            std::cout << (i + 1) << ". " << choices[i] << std::endl;
        }
    }

    int inputi(const std::string& prompt) const {
        while (true) {
            if (!prompt.empty()) {
                std::cout << prompt;
            }
            
            std::string input;
            std::getline(std::cin, input);
            
            // Check if input is empty
            if (input.empty()) {
                std::cout << "Error: Please enter a number." << std::endl;
                continue;
            }
            
            // Check if input contains only digits (and optional leading negative sign)
            bool isValid = true;
            size_t startPos = 0;
            
            // Allow negative numbers
            if (input[0] == '-') {
                if (input.length() == 1) {
                    isValid = false;
                } else {
                    startPos = 1;
                }
            }
            
            for (size_t i = startPos; i < input.length() && isValid; ++i) {
                if (!std::isdigit(input[i])) {
                    isValid = false;
                }
            }
            
            if (!isValid) {
                std::cout << "Error: Invalid input. Please enter a valid number." << std::endl;
                continue;
            }
            
            // Convert to int
            try {
                int result = std::stoi(input);
                return result;
            } catch (const std::out_of_range&) {
                std::cout << "Error: Number is too large. Please enter a smaller number." << std::endl;
            } catch (const std::invalid_argument&) {
                std::cout << "Error: Invalid input. Please enter a valid number." << std::endl;
            }
        }
    }

    int inputi() const {
        return inputi("");
    }

    // Read a full line string input
    std::string inputs(const std::string& prompt) const {
        if (!prompt.empty()) {
            std::cout << prompt;
        }
        std::string s;
        std::getline(std::cin, s);
        return s;
    }

    // Prompt for yes/no and return true for 'y'/'Y', false for 'n'/'N'
    bool inputYesNo(const std::string& prompt) const {
        while (true) {
            if (!prompt.empty()) {
                std::cout << prompt;
            }
            std::string s;
            std::getline(std::cin, s);
            if (!s.empty()) {
                char c = static_cast<char>(std::tolower(static_cast<unsigned char>(s[0])));
                if (c == 'y') return true;
                if (c == 'n') return false;
            }
            std::cout << "Please enter 'y' or 'n'." << std::endl;
        }
    }

    void clear() const {
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
    }

    std::string center(const std::string& toOutput, int width) const {
        int len = static_cast<int>(toOutput.length());
        if (len >= width) {
            return toOutput; // No centering needed
        }
        int padding = (width - len) / 2;
        return std::string(padding, ' ') + toOutput;
    }

    void header(std::string toOutput)
    {
        const int borderWidth = 43; // Width of the border line
        this->coutln("===========================================");
        this->coutln(this->center(toOutput, borderWidth));
        this->coutln("===========================================");
    }

    void subheader(const std::string& toOutput)
    {
        const int borderWidth = 43;
        this->coutln(std::string(borderWidth, '-'));
        int textLen = static_cast<int>(toOutput.length());
        int totalPadding = borderWidth - textLen;
        int leftPadding = totalPadding / 2;
        int rightPadding = totalPadding - leftPadding;
        if (leftPadding < 0) leftPadding = 0;
        if (rightPadding < 0) rightPadding = 0;
        std::string centeredLine = std::string(leftPadding, ' ') + toOutput + std::string(rightPadding, ' ');
        this->coutln(centeredLine);
        this->coutln(std::string(borderWidth, '-'));
    }

    // Note: single overload to avoid ambiguity

    void br()
    {
        std::cout << std::endl;
    }

    // JSON utility methods
    std::string escapeJson(const std::string& s) const {
        std::string result;
        for (char c : s) {
            if (c == '"') result += "\\\"";
            else if (c == '\\') result += "\\\\";
            else if (c == '\n') result += "\\n";
            else if (c == '\r') result += "\\r";
            else if (c == '\t') result += "\\t";
            else result += c;
        }
        return result;
    }

    std::string unescapeJson(const std::string& s) const {
        std::string result;
        for (size_t i = 0; i < s.length(); ++i) {
            if (s[i] == '\\' && i + 1 < s.length()) {
                char next = s[i + 1];
                if (next == '"' || next == '\\' || next == '/') result += next;
                else if (next == 'n') result += '\n';
                else if (next == 'r') result += '\r';
                else if (next == 't') result += '\t';
                else result += next;
                ++i;
            } else {
                result += s[i];
            }
        }
        return result;
    }

    std::string extractJsonString(const std::string& json, const std::string& key) const {
        std::string searchKey = "\"" + key + "\"";
        size_t pos = json.find(searchKey);
        if (pos == std::string::npos) return "";
        pos = json.find(":", pos);
        if (pos == std::string::npos) return "";
        pos = json.find("\"", pos);
        if (pos == std::string::npos) return "";
        size_t end = pos + 1;
        while (end < json.length()) {
            if (json[end] == '"' && (end == 0 || json[end - 1] != '\\')) break;
            ++end;
        }
        return unescapeJson(json.substr(pos + 1, end - pos - 1));
    }

    std::vector<std::string> extractJsonArray(const std::string& json, const std::string& key) const {
        std::vector<std::string> result;
        std::string searchKey = "\"" + key + "\"";
        size_t pos = json.find(searchKey);
        if (pos == std::string::npos) return result;
        pos = json.find("[", pos);
        if (pos == std::string::npos) return result;
        size_t end = json.find("]", pos);
        if (end == std::string::npos) return result;
        std::string arrayContent = json.substr(pos + 1, end - pos - 1);
        
        size_t i = 0;
        while (i < arrayContent.length()) {
            while (i < arrayContent.length() && (arrayContent[i] == ' ' || arrayContent[i] == ',')) ++i;
            if (i >= arrayContent.length()) break;
            if (arrayContent[i] == '"') {
                size_t start = i + 1;
                ++i;
                while (i < arrayContent.length()) {
                    if (arrayContent[i] == '"' && (i == 0 || arrayContent[i - 1] != '\\')) break;
                    ++i;
                }
                result.push_back(unescapeJson(arrayContent.substr(start, i - start)));
                ++i;
            }
        }
        return result;
    }
};

inline Out out{};