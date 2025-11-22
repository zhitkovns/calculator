#include "core/Calculator.h"
#include <iostream>
#include <string>
#include <stdexcept>

// Константы для лучшей поддерживаемости
namespace Command {
    constexpr const char* HELP = "/help";
    constexpr const char* LIST = "/list"; 
    constexpr const char* EXIT = "/exit";
    constexpr const char* QUIT = "/quit";
}

void printHelp() {
    std::cout << "\n=== Calculator ===" << std::endl;
    std::cout << "Supported operations: +, -, *, /, ()" << std::endl;
    std::cout << "Plugins are loaded automatically from ./plugins/" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  /help  - show this help" << std::endl;
    std::cout << "  /list  - list all available operations" << std::endl;
    std::cout << "  /exit  - exit the program" << std::endl;
    std::cout << "Expression examples:" << std::endl;
    std::cout << "  2 + 3 * 4      -> 14" << std::endl;
    std::cout << "  (2 + 3) * 4    -> 20" << std::endl;
    std::cout << "  -5 + 10        -> 5" << std::endl;
    std::cout << "===================\n" << std::endl;
}

// Функция для обработки специальных команд
bool handleCommand(const std::string& input, Calculator& calculator) {
    if (input == Command::HELP) {
        printHelp();
        return true;
    }
    else if (input == Command::LIST) {
        auto operations = calculator.getAvailableOperations();
        std::cout << "Available operations (" << operations.size() << "): ";
        for (size_t i = 0; i < operations.size(); ++i) {
            std::cout << operations[i];
            if (i < operations.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
        return true;
    }
    else if (input == Command::EXIT || input == Command::QUIT) {
        std::cout << "Exiting program..." << std::endl;
        return false;
    }
    
    std::cout << "Unknown command. Type /help for help." << std::endl;
    return true;
}

int main() {
    std::cout << "Starting calculator..." << std::endl;
    
    try {
        Calculator calculator;
        calculator.initialize();
        
        std::cout << "Calculator is ready!" << std::endl;
        std::cout << "Enter expression or /help for help\n" << std::endl;
        
        std::string input;
        while (true) {
            std::cout << "> ";
            
            if (!std::getline(std::cin, input)) {
                break; // EOF или ошибка ввода
            }
            
            // Пропускаем пустые строки
            if (input.empty()) continue;
            
            // Обработка команд
            if (input[0] == '/') {
                if (!handleCommand(input, calculator)) break;
                continue;
            }
            
            // Вычисление выражения
            try {
                double result = calculator.calculate(input);
                std::cout << "Result: " << result << std::endl;
            }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Critical error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}