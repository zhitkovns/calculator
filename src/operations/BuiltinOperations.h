#pragma once
#include "IOperation.h"
#include <cmath>

// Базовый класс для встроенных операций
class BaseOperation : public IOperation {
protected:
    std::string name_;
    OperationType type_;
    int priority_;
    size_t argCount_;
    
public:
    BaseOperation(const std::string& name, OperationType type, int priority, size_t argCount)
        : name_(name), type_(type), priority_(priority), argCount_(argCount) {}
    
    std::string getName() const override { return name_; }
    OperationType getType() const override { return type_; }
    int getPriority() const override { return priority_; }
    size_t getArgumentCount() const override { return argCount_; }
};

// Конкретные встроенные операции

class AddOperation : public BaseOperation {
public:
    AddOperation() : BaseOperation("+", OperationType::BINARY, 1, 2) {}
    double execute(const std::vector<double>& args) override;
};

class SubtractOperation : public BaseOperation {
public:
    SubtractOperation() : BaseOperation("-", OperationType::BINARY, 1, 2) {}
    double execute(const std::vector<double>& args) override;
};

class MultiplyOperation : public BaseOperation {
public:
    MultiplyOperation() : BaseOperation("*", OperationType::BINARY, 2, 2) {}
    double execute(const std::vector<double>& args) override;
};

class DivideOperation : public BaseOperation {
public:
    DivideOperation() : BaseOperation("/", OperationType::BINARY, 2, 2) {}
    double execute(const std::vector<double>& args) override;
};

class PowerOperation : public BaseOperation {
public:
    PowerOperation() : BaseOperation("^", OperationType::BINARY, 3, 2) {}
    double execute(const std::vector<double>& args) override;
};

// Унарный минус
class UnaryMinusOperation : public BaseOperation {
public:
    UnaryMinusOperation() : BaseOperation("-", OperationType::UNARY, 4, 1) {}
    double execute(const std::vector<double>& args) override;
};

// Скобки
class LeftParenthesis : public BaseOperation {
public:
    LeftParenthesis() : BaseOperation("(", OperationType::FUNCTION, 0, 0) {}
    double execute(const std::vector<double>& args) override;
};

class RightParenthesis : public BaseOperation {
public:
    RightParenthesis() : BaseOperation(")", OperationType::FUNCTION, 0, 0) {}
    double execute(const std::vector<double>& args) override;
};