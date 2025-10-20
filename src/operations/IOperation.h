#pragma once
#include <string>
#include <vector>
#include <memory>

// Типы операций для парсера
enum class OperationType {
    BINARY,     // +, -, *, /, ^
    UNARY,      // - (унарный минус)
    FUNCTION    // sin, cos, ln и т.д.
};

// Базовый интерфейс для всех операций калькулятора
class IOperation {
public:
    virtual ~IOperation() = default;
    
    // Выполнение операции с заданными аргументами
    virtual double execute(const std::vector<double>& args) = 0;
    
    // Имя операции
    virtual std::string getName() const = 0;
    
    // Тип операции (для парсера)
    virtual OperationType getType() const = 0;
    
    // Приоритет операции
    virtual int getPriority() const = 0;
    
    // Количество аргументов (для валидации)
    virtual size_t getArgumentCount() const = 0;
};

// Для удобства
using OperationPtr = std::unique_ptr<IOperation>;