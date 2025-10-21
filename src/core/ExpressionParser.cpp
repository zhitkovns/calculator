#include "ExpressionParser.h"
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
            throw std::invalid_argument("There are no tokens for parsing");
        }
        
        size_t index = 0;
        auto result = parseExpression(tokens, index);
        
        if (index != tokens.size()) {
            throw std::runtime_error("Undecompressed tokens in the expression");
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
        
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '(' || c == ')') {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            tokens.push_back(std::string(1, c));
        } else {
            currentToken += c;
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
        
        if (!isOperator(token) || token == ")") {
            break;
        }
        
        IOperation* op = operationFactory_.getOperation(token);
        if (!op || op->getType() != OperationType::BINARY) {
            break;
        }
        
        // Для операций + и - проверяем приоритет
        if (op->getPriority() > 1) { // Если приоритет выше чем у +/-
            break;
        }
        
        index++;
        
        auto right = parseTerm(tokens, index);
        left = std::make_unique<BinaryNode>(std::move(left), std::move(right), op);
    }
    
    return left;
}

std::unique_ptr<Node> ExpressionParser::parseTerm(const std::vector<std::string>& tokens, size_t& index) {
    auto left = parseFactor(tokens, index);
    
    while (index < tokens.size()) {
        std::string token = tokens[index];
        
        if (!isOperator(token) || token == ")") {
            break;
        }
        
        IOperation* op = operationFactory_.getOperation(token);
        if (!op || op->getType() != OperationType::BINARY) {
            break;
        }
        
        // Только операции * и / (приоритет 2)
        if (op->getPriority() != 2) {
            break;
        }
        
        index++;
        
        auto right = parseFactor(tokens, index);
        left = std::make_unique<BinaryNode>(std::move(left), std::move(right), op);
    }
    
    return left;
}

std::unique_ptr<Node> ExpressionParser::parseFactor(const std::vector<std::string>& tokens, size_t& index) {
    if (index >= tokens.size()) {
        throw std::runtime_error("The unexpected end of an expression");
    }
    
    std::string token = tokens[index];
    
    if (isNumber(token)) {
        double value = std::stod(token);
        index++;
        return std::make_unique<NumberNode>(value);
    }
    
    if (token == "(") {
        index++; // Потребляем "("
        auto expr = parseExpression(tokens, index);
        
        if (index >= tokens.size() || tokens[index] != ")") {
            throw std::runtime_error("A closing parenthesis was expected");
        }
        index++; // Потребляем ")"
        
        return expr;
    }
    
    // Обработка унарного минуса
    if (token == "-") {
        // Проверяем, является ли это унарным минусом
        if (index == 0 || tokens[index - 1] == "(" || isOperator(tokens[index - 1])) {
            index++; // Потребляем "-"
            IOperation* unaryMinus = operationFactory_.getOperation("unary_minus");
            if (!unaryMinus) {
                throw std::runtime_error("The unary minus sign is not registered");
            }
            auto operand = parseFactor(tokens, index);
            return std::make_unique<UnaryNode>(std::move(operand), unaryMinus);
        }
    }
    
    IOperation* func = operationFactory_.getOperation(token);
    if (func && func->getType() == OperationType::FUNCTION) {
        // TODO: Реализовать парсинг функций
        throw std::runtime_error("The functions are not supported yet: " + token);
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
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "^";
}