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
    bool rightAssociative_;
    
public:
    BaseOperation(const std::string& name, OperationType type, int priority, size_t argCount, bool rightAssoc = false)
        : name_(name), type_(type), priority_(priority), argCount_(argCount), rightAssociative_(rightAssoc) {}
    
    std::string getName() const override { return name_; }
    OperationType getType() const override { return type_; }
    int getPriority() const override { return priority_; }
    size_t getArgumentCount() const override { return argCount_; }
    bool isRightAssociative() const override { return rightAssociative_; }
};

// Конкретные встроенные операции - ТОЛЬКО БАЗОВЫЕ
class AddOperation : public BaseOperation {
public:
    AddOperation() : BaseOperation("+", OperationType::BINARY, 1, 2, false) {}
    double execute(const std::vector<double>& args) override;
};

class SubtractOperation : public BaseOperation {
public:
    SubtractOperation() : BaseOperation("-", OperationType::BINARY, 1, 2, false) {}
    double execute(const std::vector<double>& args) override;
};

class MultiplyOperation : public BaseOperation {
public:
    MultiplyOperation() : BaseOperation("*", OperationType::BINARY, 2, 2, false) {}
    double execute(const std::vector<double>& args) override;
};

class DivideOperation : public BaseOperation {
public:
    DivideOperation() : BaseOperation("/", OperationType::BINARY, 2, 2, false) {}
    double execute(const std::vector<double>& args) override;
};

// Скобки (специальная операция)
class LeftParenthesis : public BaseOperation {
public:
    LeftParenthesis() : BaseOperation("(", OperationType::FUNCTION, 0, 0, false) {}
    double execute(const std::vector<double>& args) override;
};

class RightParenthesis : public BaseOperation {
public:
    RightParenthesis() : BaseOperation(")", OperationType::FUNCTION, 0, 0, false) {}
    double execute(const std::vector<double>& args) override;
};