#include "ExpressionParser.h"
#include "AST/FunctionNode.h"
#include "operations/wrappers/ExtensionOperationWrapper.h"
#include <sstream>
#include <cctype>
#include <stdexcept>
#include <iostream>

ExpressionParser::ExpressionParser(const OperationFactory& factory) 
    : operationFactory_(factory) {
}

std::unique_ptr<Node> ExpressionParser::parse(const std::string& expression) {
    if (expression.empty()) {
        throw std::invalid_argument("Empty expression");
    }

    try {
        // Tokenize expression
        auto tokens = tokenize(expression);
        
        if (tokens.empty()) {
            throw std::invalid_argument("No tokens to parse");
        }
        
        // Parse tokens to AST
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
        
        // Handle operators from plugins with priority 1 (+, -)
        IOperation* op = operationFactory_.getOperation(token);
        if (op && op->getType() == OperationType::BINARY && op->getPriority() == 1) {
            index++; // Consume operator
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
        
        // Handle operators based on their priority
        if (op->getPriority() == 2) { // *, /
            index++; // Consume operator
            auto right = parseFactor(tokens, index);
            left = std::make_unique<BinaryNode>(std::move(left), std::move(right), op);
        } 
        else if (op->getPriority() == 3) { // ^
            index++; // Consume operator
            
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
    
    // Handle numbers
    if (isNumber(token)) {
        double value = std::stod(token);
        index++;
        return std::make_unique<NumberNode>(value);
    }
    
    // Handle parentheses
    if (token == "(") {
        index++; // Consume "("
        auto expr = parseExpression(tokens, index);
        
        if (index >= tokens.size() || tokens[index] != ")") {
            throw std::runtime_error("Expected closing parenthesis");
        }
        index++; // Consume ")"
        
        return expr;
    }
    
    // Handle unary minus
    if (token == "-") {
        // Check if this is unary minus context
        if (index == 0 || tokens[index - 1] == "(" || isOperator(tokens[index - 1])) {
            index++; // Consume "-"
            
            // Get unary minus operation from plugin
            IOperation* unaryMinus = operationFactory_.getOperation("unary_minus");
            if (!unaryMinus) {
                throw std::runtime_error("Unary minus operation not found");
            }
            
            auto operand = parseFactor(tokens, index);
            return std::make_unique<UnaryNode>(std::move(operand), unaryMinus);
        }
    }
    
    // Handle other unary operations from plugins
    IOperation* unaryOp = operationFactory_.getOperation(token);
    if (unaryOp && unaryOp->getType() == OperationType::UNARY) {
        // Check if this is actually a unary context
        if (index == 0 || tokens[index - 1] == "(" || isOperator(tokens[index - 1])) {
            index++; // Consume operator
            auto operand = parseFactor(tokens, index);
            return std::make_unique<UnaryNode>(std::move(operand), unaryOp);
        }
    }
    
    // Handle functions from plugins
    IOperation* func = operationFactory_.getOperation(token);
    if (func && func->getType() == OperationType::FUNCTION) {
        index++; // Consume function name
        
        // Check for opening parenthesis
        if (index >= tokens.size() || tokens[index] != "(") {
            throw std::runtime_error("Expected '(' after function " + token);
        }
        index++; // Consume "("
        
        // Parse argument
        auto argument = parseExpression(tokens, index);
        
        // Check for closing parenthesis
        if (index >= tokens.size() || tokens[index] != ")") {
            throw std::runtime_error("Expected ')' after function argument");
        }
        index++; // Consume ")"
        
        return std::make_unique<UnaryNode>(std::move(argument), func);
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
    if (!op) return false;
    
    // Consider binary operations as operators
    if (op->getType() == OperationType::BINARY) {
        return true;
    }
    
    // Also consider unary operations that can be used as operators
    if (op->getType() == OperationType::UNARY && op->getArgumentCount() == 1) {
        return true;
    }
    
    return false;
}