#include "BuiltinOperations.h"
#include <stdexcept>

// Реализации встроенных операций

double AddOperation::execute(const std::vector<double>& args) {
    if (args.size() != 2) throw std::invalid_argument("Add requires 2 arguments");
    return args[0] + args[1];
}

double SubtractOperation::execute(const std::vector<double>& args) {
    if (args.size() != 2) throw std::invalid_argument("Subtract requires 2 arguments");
    return args[0] - args[1];
}

double MultiplyOperation::execute(const std::vector<double>& args) {
    if (args.size() != 2) throw std::invalid_argument("Multiply requires 2 arguments");
    return args[0] * args[1];
}

double DivideOperation::execute(const std::vector<double>& args) {
    if (args.size() != 2) throw std::invalid_argument("Divide requires 2 arguments");
    if (args[1] == 0) throw std::runtime_error("Division by zero");
    return args[0] / args[1];
}

// УДАЛЕНО: PowerOperation::execute - теперь плагин
// УДАЛЕНО: UnaryMinusOperation::execute - теперь плагин

// Скобки не выполняют вычисления, их обрабатывает парсер
double LeftParenthesis::execute(const std::vector<double>& args) {
    throw std::runtime_error("Parenthesis should not be executed directly");
}

double RightParenthesis::execute(const std::vector<double>& args) {
    throw std::runtime_error("Parenthesis should not be executed directly");
}