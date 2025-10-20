#pragma once
#include "operations/IOperation.h"
#include <unordered_map>
#include <memory>
#include <string>

// Фабрика для управления всеми операциями (встроенные + плагины)
class OperationFactory {
private:
    std::unordered_map<std::string, IOperation*> operations_;

public:
    OperationFactory() = default;
    
    // Регистрация операции
    void registerOperation(const std::string& name, IOperation* operation);
    
    // Получение операции по имени
    IOperation* getOperation(const std::string& name) const;
    
    // Проверка существования операции
    bool hasOperation(const std::string& name) const;
    
    // Получение всех зарегистрированных операций
    std::vector<std::string> getAvailableOperations() const;
    
    // Очистка всех операций
    void clear();
};