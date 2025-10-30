#include "ExpressionParser.h"
#include "AST/FunctionNode.h"
#include "operations/wrappers/ExtensionOperationWrapper.h"
#include <sstream>
#include <cctype>
#include <stdexcept>

ExpressionParser::ExpressionParser(const OperationFactory& factory) 
    : operationFactory_(factory) {
}

std::unique_ptr<Node> ExpressionParser::parse(const std::string& expression) {
    if (expression.empty()) {
        throw std::invalid_argument("Empty expression");
    }

    try {
        // Токенизируем выражение
        auto tokens = tokenize(expression);
        
        if (tokens.empty()) {
            throw std::invalid_argument("No tokens to parse");
        }
        
        // Парсим токены в AST
        size_t index = 0;
        auto result = parseExpression(tokens, index);
        
        if (index != tokens.size()) {
            std::string availableOps;
            auto allOps = operationFactory_.getAvailableOperations();
            for (const auto& op : allOps) {
                availableOps += op + " ";
            }
            throw std::runtime_error("Unprocessed tokens in expression. Last token: '" + 
                                   (index < tokens.size() ? tokens[index] : "none") + 
                                   "'. Available operations: " + availableOps);
        }
        
        return result;
        
    } catch (const std::exception& e) {
        throw std::runtime_error("Parsing error: " + std::string(e.what()));
    }
}

std::vector<std::string> ExpressionParser::tokenize(const std::string& expression) {
    std::vector<std::string> tokens;
    std::string currentToken;
    
    for (size_t i = 0; i < expression.length(); ++i) {
        char c = expression[i];
        
        // Проверка пробела
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            continue;
        }
        
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '(' || c == ')' || c == ',') {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            tokens.push_back(std::string(1, c));
        } 
        else if (std::isdigit(c) || c == '.' || 
                 (c == '-' && currentToken.empty() && (i == 0 || expression[i-1] == '(' || isOperator(std::string(1, expression[i-1]))))) {
            currentToken += c;
        }
        // Проверка буквы
        else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            currentToken += c;
        }
        else {
            throw std::runtime_error("Invalid character in expression: " + std::string(1, c));
        }
    }
    
    if (!currentToken.empty()) {
        tokens.push_back(currentToken);
    }
    
    return tokens;
}

std::unique_ptr<Node> ExpressionParser::parseExpression(const std::vector<std::string>& tokens, size_t& index) {
    auto left = parseTerm(tokens, index);
    
    while (index < tokens.size()) {
        std::string token = tokens[index];
        
        // Обрабатывать операторы из плагинов с приоритетом 1 (+, -)
        IOperation* op = operationFactory_.getOperation(token);
        if (op && op->getType() == OperationType::BINARY && op->getPriority() == 1) {
            index++;
            auto right = parseTerm(tokens, index);
            left = std::make_unique<BinaryNode>(std::move(left), std::move(right), op);
        } else {
            break;
        }
    }
    
    return left;
}

std::unique_ptr<Node> ExpressionParser::parseTerm(const std::vector<std::string>& tokens, size_t& index) {
    auto left = parseFactor(tokens, index);
    
    while (index < tokens.size()) {
        std::string token = tokens[index];
        
        IOperation* op = operationFactory_.getOperation(token);
        if (!op || op->getType() != OperationType::BINARY) {
            break;
        }
        
        // Обрабатываем запросы операторов в соответствии с их приоритетом
        if (op->getPriority() == 2) { // *, /
            index++;
            auto right = parseFactor(tokens, index);
            left = std::make_unique<BinaryNode>(std::move(left), std::move(right), op);
        } 
        else if (op->getPriority() == 3) { // ^
            index++;
            
            // Для лево-ассоциативных операторов, парсим правую сторону как Factor
            if (!op->isRightAssociative()) {
                auto right = parseFactor(tokens, index);  // Лево-ассоциативно: a^b^c = (a^b)^c
                left = std::make_unique<BinaryNode>(std::move(left), std::move(right), op);
            } else {
                auto right = parseTerm(tokens, index);    // Право-ассоциативно: a^b^c = a^(b^c)
                left = std::make_unique<BinaryNode>(std::move(left), std::move(right), op);
            }
        }
        else {
            break;
        }
    }
    
    return left;
}

std::unique_ptr<Node> ExpressionParser::parseFactor(const std::vector<std::string>& tokens, size_t& index) {
    if (index >= tokens.size()) {
        throw std::runtime_error("Unexpected end of expression");
    }
    
    std::string token = tokens[index];
    
    // Обрабатываем числа
    if (isNumber(token)) {
        double value = stringToDouble(token);
        index++;
        return std::make_unique<NumberNode>(value);
    }
    
    // Обрабатываем скобки
    if (token == "(") {
        index++; // "("
        auto expr = parseExpression(tokens, index);
        
        if (index >= tokens.size() || tokens[index] != ")") {
            throw std::runtime_error("Expected closing parenthesis");
        }
        index++; // ")"
        
        return expr;
    }
    
    // Обрабатываем унарный минус
    if (token == "-") {
        // Является ли унарным минусом
        if (index == 0 || tokens[index - 1] == "(" || isOperator(tokens[index - 1])) {
            index++; // "-"
            
            // Получаем унарный минус из плагинов
            IOperation* unaryMinus = operationFactory_.getOperation("unary_minus");
            if (!unaryMinus) {
                throw std::runtime_error("Unary minus operation not found");
            }
            
            auto operand = parseFactor(tokens, index);
            return std::make_unique<UnaryNode>(std::move(operand), unaryMinus);
        }
    }
    
    // Обрабатываем другие унарные операции из плагинов
    IOperation* unaryOp = operationFactory_.getOperation(token);
    if (unaryOp && unaryOp->getType() == OperationType::UNARY) {
        // Является ли унарным
        if (index == 0 || tokens[index - 1] == "(" || isOperator(tokens[index - 1])) {
            index++;
            auto operand = parseFactor(tokens, index);
            return std::make_unique<UnaryNode>(std::move(operand), unaryOp);
        }
    }
    
    // Обрабатываем функции из плагинов
    IOperation* func = operationFactory_.getOperation(token);
    if (func && func->getType() == OperationType::FUNCTION) {
        index++; // Имя функции
        
        // Проверка на открытые скобки
        if (index >= tokens.size() || tokens[index] != "(") {
            throw std::runtime_error("Expected '(' after function " + token);
        }
        index++; // "("

        auto argument = parseExpression(tokens, index);
        
        // Проверка на закрывающие скобки
        if (index >= tokens.size() || tokens[index] != ")") {
            throw std::runtime_error("Expected ')' after function argument");
        }
        index++; // ")"
        
        return std::make_unique<UnaryNode>(std::move(argument), func);
    }
    
    throw std::runtime_error("Unknown token: " + token);
}

bool ExpressionParser::isNumber(const std::string& token) const {
    if (token.empty()) return false;
    
    bool hasDecimalPoint = false;
    bool hasDigits = false;
    
    for (size_t i = 0; i < token.length(); ++i) {
        char c = token[i];
        
        if (c == '-' && i == 0) {
            // Разрешаем минус в начале
            continue;
        }
        else if (c == '.') {
            if (hasDecimalPoint) {
                return false; // Две точки не допускаются
            }
            hasDecimalPoint = true;
        }
        else if (std::isdigit(c)) {
            hasDigits = true;
        }
        else {
            return false; // Недопустимый символ
        }
    }
    
    return hasDigits; // Должна быть хотя бы одна цифра
}

double ExpressionParser::stringToDouble(const std::string& str) const {
    std::istringstream iss(str);
    double value;
    iss >> value;
    
    if (iss.fail() || !iss.eof()) {
        throw std::runtime_error("Invalid number format: " + str);
    }
    
    return value;
}

bool ExpressionParser::isOperator(const std::string& token) const {
    IOperation* op = operationFactory_.getOperation(token);
    if (!op) return false;
    
    // Рассматриваем бинарные операции как операторы
    if (op->getType() == OperationType::BINARY) {
        return true;
    }
    
    // Также рассматриваем унарные операции как операторы
    if (op->getType() == OperationType::UNARY && op->getArgumentCount() == 1) {
        return true;
    }
    
    return false;
}