#include "Calculator.h"
#include "operations/wrappers/ExtensionOperationWrapper.h"
#include <iostream>
#include <stdexcept>
#include <unordered_set>

Calculator::Calculator() {
    registerBuiltinOperations();
}

void Calculator::initialize() {
    try {
        // Загружаем плагины из папки ./plugins/
        extensionRegistry_.scanExtensionsDirectory("./plugins/");
        
        // Сохраним количество built-in операций до регистрации плагинов
        size_t builtinCountBefore = operationFactory_.getAvailableOperations().size();
        
        // Регистрируем функции из плагинов в фабрике операций
        auto availableExtensions = extensionRegistry_.getAvailableExtensions();
        
        // Временный набор для уникальных имен (только основные)
        std::unordered_set<std::string> uniqueFunctionNames;
        
        for (const auto& funcName : availableExtensions) {
            auto extension = extensionRegistry_.findExtension(funcName);
            if (extension) {
                // Берем только основное имя функции
                std::string primaryName = extension->getPrimaryName();
                
                // Регистрируем только если еще не зарегистрировано
                if (uniqueFunctionNames.find(primaryName) == uniqueFunctionNames.end()) {
                    uniqueFunctionNames.insert(primaryName);
                    
                    // Создаем обертку для функции плагина
                    auto wrapper = std::make_shared<ExtensionOperationWrapper>(extension, primaryName);
                    // Сохраняем обертку и регистрируем в фабрике
                    extensionWrappers_.push_back(wrapper);
                    operationFactory_.registerOperation(primaryName, wrapper.get());
                }
            }
        }
        
        // Создаем парсер с готовой фабрикой операций
        parser_ = std::make_unique<ExpressionParser>(operationFactory_);
        
        std::cout << "The calculator is initialized. " 
                  << "Operations: " << getAvailableOperations().size() 
                  << " (built-in: " << builtinCountBefore
                  << ", plugins: " << uniqueFunctionNames.size() << ")" << std::endl;
                  
    } catch (const std::exception& e) {
        std::cerr << "Calculator initialization error: " << e.what() << std::endl;
        throw;
    }
}

void Calculator::registerBuiltinOperations() {
    // Регистрируем только базовые встроенные операции в фабрике
    static AddOperation addOp;
    static SubtractOperation subOp;
    static MultiplyOperation mulOp;
    static DivideOperation divOp;
    static LeftParenthesis leftParen;
    static RightParenthesis rightParen;
    
    operationFactory_.registerOperation("+", &addOp);
    operationFactory_.registerOperation("-", &subOp);
    operationFactory_.registerOperation("*", &mulOp);
    operationFactory_.registerOperation("/", &divOp);
    operationFactory_.registerOperation("(", &leftParen);
    operationFactory_.registerOperation(")", &rightParen);

}

double Calculator::calculate(const std::string& expression) {
    if (expression.empty()) {
        throw std::invalid_argument("Empty expression");
    }

    if (!parser_) {
        throw std::runtime_error("The calculator is not initialized. Call initialize()");
    }

    try {
        return parser_->parse(expression)->evaluate();
        
    } catch (const std::exception& e) {
        throw std::runtime_error("Calculation error: " + std::string(e.what()));
    }
}

std::vector<std::string> Calculator::getAvailableOperations() const {
    auto builtinOps = operationFactory_.getAvailableOperations();
    auto extensionOps = extensionRegistry_.getAvailableExtensions();
    
    // Используем set для устранения дубликатов
    std::unordered_set<std::string> uniqueOps;
    
    for (const auto& op : builtinOps) {
        uniqueOps.insert(op);
    }
    for (const auto& op : extensionOps) {
        uniqueOps.insert(op);
    }
    
    return std::vector<std::string>(uniqueOps.begin(), uniqueOps.end());
}

bool Calculator::hasOperation(const std::string& name) const {
    return operationFactory_.hasOperation(name) || extensionRegistry_.extensionExists(name);
}