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

// Базовый интерфейс для ВСЕХ операций калькулятора
class IOperation {
public:
    virtual ~IOperation() = default;
    
    // Выполнение операции с заданными аргументами
    virtual double execute(const std::vector<double>& args) = 0;
    
    // Имя операции (для идентификации в выражениях)
    virtual std::string getName() const = 0;
    
    // Тип операции (для парсера)
    virtual OperationType getType() const = 0;
    
    // Приоритет операции (чем выше, тем раньше выполняется)
    virtual int getPriority() const = 0;
    
    // Количество аргументов (для валидации)
    virtual size_t getArgumentCount() const = 0;
    
    // Право-ассоциативность операции
    virtual bool isRightAssociative() const = 0;
};

// Умный указатель для удобства
using OperationPtr = std::unique_ptr<IOperation>;