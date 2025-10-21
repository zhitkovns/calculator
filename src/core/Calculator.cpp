#include "Calculator.h"
#include <iostream>
#include <stdexcept>

Calculator::Calculator() {
    registerBuiltinOperations();
}

void Calculator::initialize() {
    try {
        // Загружаем плагины из папки ./plugins/
        pluginManager_.loadPlugins();
        
        // Регистрируем операции из плагинов в фабрике
        for (const auto& opName : pluginManager_.getAvailableOperations()) {
            IOperation* op = pluginManager_.getOperation(opName);
            if (op) {
                operationFactory_.registerOperation(opName, op);
            }
        }
        
        // Создаем парсер с готовой фабрикой операций
        parser_ = std::make_unique<ExpressionParser>(operationFactory_);
        
        std::cout << "The calculator is initialized. " 
                  << "Operations: " << getAvailableOperations().size() 
                  << " (built-in: " << operationFactory_.getAvailableOperations().size() - pluginManager_.getAvailableOperations().size()
                  << ", plugins: " << pluginManager_.getAvailableOperations().size() << ")" << std::endl;
                  
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
    return operationFactory_.getAvailableOperations();
}

bool Calculator::hasOperation(const std::string& name) const {
    return operationFactory_.hasOperation(name);
}