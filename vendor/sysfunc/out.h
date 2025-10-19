#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <limits>
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

    void clear() const {
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
    }

    void header(std::string toOutput)
    {
        const int borderWidth = 43; // Width of the border line
        this->coutln("===========================================");
        
        // Calculate padding for centering
        int textLen = static_cast<int>(toOutput.length());
        int totalPadding = borderWidth - textLen;
        int leftPadding = totalPadding / 2;
        int rightPadding = totalPadding - leftPadding;
        
        // Ensure non-negative padding
        if (leftPadding < 0) leftPadding = 0;
        if (rightPadding < 0) rightPadding = 0;
        
        // Print centered text
        std::string centeredLine = std::string(leftPadding, ' ') + toOutput + std::string(rightPadding, ' ');
        this->coutln(centeredLine);
        
        this->coutln("===========================================");
    }

    void br()
    {
        std::cout << std::endl;
    }
};

inline Out out{};