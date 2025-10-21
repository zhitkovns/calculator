#pragma once
#include "operations/IOperation.h"
#include "operations/BuiltinOperations.h"
#include "plugins/PluginManager.h"
#include "ExpressionParser.h"
#include "OperationFactory.h"
#include <memory>
#include <string>
#include <unordered_map>

// Главный класс калькулятора - оркестратор всей системы
class Calculator {
private:
    PluginManager pluginManager_;
    OperationFactory operationFactory_;
    std::unique_ptr<ExpressionParser> parser_;
    
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
};