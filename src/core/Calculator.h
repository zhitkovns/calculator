#pragma once
#include "operations/IOperation.h"
#include "operations/BuiltinOperations.h"
#include "plugins/ExtensionRegistry.h"
#include "ExpressionParser.h"
#include "OperationFactory.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// Предварительное объявление чтобы избежать циклических зависимостей
class ExtensionOperationWrapper;

// Главный класс калькулятора - оркестратор всей системы
class Calculator {
private:
    ExtensionRegistry extensionRegistry_;
    OperationFactory operationFactory_;
    std::unique_ptr<ExpressionParser> parser_;
    std::vector<std::shared_ptr<ExtensionOperationWrapper>> extensionWrappers_;
    
public:
    Calculator();
    ~Calculator() = default;

    Calculator(const Calculator&) = delete;
    Calculator& operator=(const Calculator&) = delete;

    void initialize();

    double calculate(const std::string& expression);

    std::vector<std::string> getAvailableOperations() const;

    bool hasOperation(const std::string& name) const;

private:
    void registerBuiltinOperations();
    double normalizeZero(double value) const;
};