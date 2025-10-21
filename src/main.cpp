#include "core/Calculator.h"
#include <iostream>
#include <string>
#include <stdexcept>

// Функция для вывода справки
void printHelp() {
    std::cout << "=== Calculator ===" << std::endl;
    std::cout << "Supported operations: +, -, *, /, ^, ()" << std::endl;
    std::cout << "Plugins are loaded automatically from the folder ./plugins/" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  /help - show this help" << std::endl;
    std::cout << "  /list - show all available operations" << std::endl;
    std::cout << "  /exit or /quit - exit the program" << std::endl;
    std::cout << "Examples of expressions:" << std::endl;
    std::cout << "  2 + 3 * 4" << std::endl;
    std::cout << "  (2 + 3) * 4" << std::endl;
    std::cout << "  2 ^ 3 + 1" << std::endl;
    std::cout << "  -5 + 10" << std::endl;
    std::cout << "===================" << std::endl;
}

// Функция для обработки специальных команд
bool handleCommand(const std::string& input, Calculator& calculator) {
    if (input == "/help") {
        printHelp();
        return true;
    }
    else if (input == "/list") {
        std::cout << "Available operations: ";
        auto operations = calculator.getAvailableOperations();
        for (size_t i = 0; i < operations.size(); ++i) {
            std::cout << operations[i];
            if (i < operations.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
        return true;
    }
    else if (input == "/exit" || input == "/quit") {
        std::cout << "Exiting the program..." << std::endl;
        return false;
    }
    
    return false;
}

int main() {
    std::cout << "Launching the calculator..." << std::endl;
    
    try {
        Calculator calculator;
        calculator.initialize();
        
        std::cout << "The calculator is ready to work!" << std::endl;
        std::cout << "Enter an expression or /help for reference" << std::endl;
        
        while (true) {
            std::cout << "> ";
            
            std::string input;
            std::getline(std::cin, input);
            
            if (input.empty()) {
                continue;
            }
            
            // Обрабатываем специальные команды
            if (input[0] == '/') {
                if (!handleCommand(input, calculator)) {
                    break;
                }
                continue;
            }
            
            try {
                double result = calculator.calculate(input);
                std::cout << "Result: " << result << std::endl;
            }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
                std::cout << "Try again or enter /help for help" << std::endl;
            }
        }
        
    }
    catch (const std::exception& e) {
        std::cerr << "Critical error when starting the calculator: " << e.what() << std::endl;
        std::cerr << "Press Enter to exit...";
        std::cin.get();
        return 1;
    }
    
    return 0;
}