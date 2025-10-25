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
        auto tokens = tokenize(expression);
        
        if (tokens.empty()) {
            throw std::invalid_argument("No tokens to parse");
        }
        
        size_t index = 0;
        auto result = parseExpression(tokens, index);
        
        if (index != tokens.size()) {
            throw std::runtime_error("Unprocessed tokens in expression. Last token: '" + 
                                   (index < tokens.size() ? tokens[index] : "none") + "'");
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
        
        if (std::isspace(static_cast<unsigned char>(c))) {
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
        else if (std::isdigit(static_cast<unsigned char>(c)) || c == '.' || 
                 (c == '-' && currentToken.empty() && (i == 0 || expression[i-1] == '(' || isOperator(std::string(1, expression[i-1]))))) {
            currentToken += c;
        }
        // Проверьте, является ли символ буквой (для имен функций)
        else if (std::isalpha(static_cast<unsigned char>(c))) {
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
        
        if (token == "+" || token == "-") {
            IOperation* op = operationFactory_.getOperation(token);
            if (!op) break;
            
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
        
        if (token == "*" || token == "/") {
            IOperation* op = operationFactory_.getOperation(token);
            if (!op) break;
            
            index++;
            auto right = parseFactor(tokens, index);
            left = std::make_unique<BinaryNode>(std::move(left), std::move(right), op);
        } 
        else if (token == "^") {
            IOperation* op = operationFactory_.getOperation(token);
            if (!op) break;
            
            index++;
            auto right = parseFactor(tokens, index);
            left = std::make_unique<BinaryNode>(std::move(left), std::move(right), op);
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
    
    if (isNumber(token)) {
        double value = std::stod(token);
        index++;
        return std::make_unique<NumberNode>(value);
    }
    
    // ПРОВЕРКА ФУНКЦИЙ
    if (std::isalpha(static_cast<unsigned char>(token[0]))) {
        IOperation* func = operationFactory_.getOperation(token);
        
        if (func && func->getType() == OperationType::FUNCTION) {
            index++;
            
            // Проверяем открывающую скобку
            if (index >= tokens.size() || tokens[index] != "(") {
                throw std::runtime_error("Expected '(' after function name: " + token);
            }
            index++;
            
            // Парсим аргументы функции
            std::vector<std::unique_ptr<Node>> arguments;
            
            if (index >= tokens.size() || tokens[index] == ")") {
                throw std::runtime_error("Function " + token + " requires arguments");
            }
            
            // Парсим первый аргумент
            arguments.push_back(parseExpression(tokens, index));
            
            // Проверяем закрывающую скобку
            if (index >= tokens.size() || tokens[index] != ")") {
                throw std::runtime_error("Expected ')' after function arguments: " + token);
            }
            index++;
            
            // Создаем узел функции
            return std::make_unique<FunctionNode>(std::move(arguments), func);
        }
    }
    
    if (token == "(") {
        index++;
        auto expr = parseExpression(tokens, index);
        
        if (index >= tokens.size() || tokens[index] != ")") {
            throw std::runtime_error("Expected closing parenthesis");
        }
        index++;
        
        return expr;
    }
    
    if (token == "-") {
        // Проверяем, является ли это унарным минусом
        if (index == 0 || (index > 0 && (tokens[index - 1] == "(" || isOperator(tokens[index - 1])))) {
            index++;
            IOperation* unaryMinus = operationFactory_.getOperation("unary_minus");
            if (!unaryMinus) {
                throw std::runtime_error("Unary minus not registered");
            }
            auto operand = parseFactor(tokens, index);
            return std::make_unique<UnaryNode>(std::move(operand), unaryMinus);
        }
    }
    
    throw std::runtime_error("Unknown token: " + token);
}

bool ExpressionParser::isNumber(const std::string& token) const {
    if (token.empty()) return false;
    
    try {
        size_t pos;
        std::stod(token, &pos);
        return pos == token.length();
    } catch (...) {
        return false;
    }
}

bool ExpressionParser::isOperator(const std::string& token) const {
    IOperation* op = operationFactory_.getOperation(token);
    return op && (op->getType() == OperationType::BINARY || op->getType() == OperationType::UNARY);
}