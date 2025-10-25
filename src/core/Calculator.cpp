#include "Calculator.h"
#include <iostream>
#include <stdexcept>

Calculator::Calculator() {
    registerBuiltinOperations();
}

void Calculator::initialize() {
    try {
        // Загружаем плагины из папки ./extensions/ через ExtensionRegistry
        extensionRegistry_.scanExtensionsDirectory("./extensions/");
        
        // Создаем парсер с готовой фабрикой операций
        parser_ = std::make_unique<ExpressionParser>(operationFactory_);
        
        std::cout << "The calculator is initialized. " 
                  << "Operations: " << getAvailableOperations().size() 
                  << " (built-in: " << operationFactory_.getAvailableOperations().size()
                  << ", extensions: " << extensionRegistry_.getAvailableExtensions().size() << ")" << std::endl;
                  
    } catch (const std::exception& e) {
        std::cerr << "Calculator initialization error: " << e.what() << std::endl;
        throw;
    }
}

void Calculator::registerBuiltinOperations() {
    static AddOperation addOp;
    static SubtractOperation subOp;
    static MultiplyOperation mulOp;
    static DivideOperation divOp;
    static PowerOperation powOp;
    static LeftParenthesis leftParen;
    static RightParenthesis rightParen;
    static UnaryMinusOperation unaryMinus;
    
    operationFactory_.registerOperation("+", &addOp);
    operationFactory_.registerOperation("-", &subOp);
    operationFactory_.registerOperation("*", &mulOp);
    operationFactory_.registerOperation("/", &divOp);
    operationFactory_.registerOperation("^", &powOp);
    operationFactory_.registerOperation("(", &leftParen);
    operationFactory_.registerOperation(")", &rightParen);
    operationFactory_.registerOperation("unary_minus", &unaryMinus);
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
    
    // Объединяем списки
    std::vector<std::string> allOps;
    allOps.insert(allOps.end(), builtinOps.begin(), builtinOps.end());
    allOps.insert(allOps.end(), extensionOps.begin(), extensionOps.end());
    
    return allOps;
}

bool Calculator::hasOperation(const std::string& name) const {
    return operationFactory_.hasOperation(name) || extensionRegistry_.extensionExists(name);
}