#pragma once
#include "AST/Node.h"
#include "AST/NumberNode.h"
#include "AST/BinaryNode.h"
#include "AST/UnaryNode.h"
#include "AST/FunctionNode.h"
#include "OperationFactory.h"
#include <memory>
#include <string>
#include <vector>
#include <stack>
#include <cctype>

// Парсер математических выражений (преобразует строку в AST)
class ExpressionParser {
private:
    const OperationFactory& operationFactory_;
    
public:
    explicit ExpressionParser(const OperationFactory& factory);
    
    std::unique_ptr<Node> parse(const std::string& expression);

private:
    // Вспомогательные методы парсера
    std::vector<std::string> tokenize(const std::string& expression);
    std::unique_ptr<Node> parseTokens(const std::vector<std::string>& tokens);
    
    // Методы рекурсивного спуска
    std::unique_ptr<Node> parseExpression(const std::vector<std::string>& tokens, size_t& index);
    std::unique_ptr<Node> parseTerm(const std::vector<std::string>& tokens, size_t& index);
    std::unique_ptr<Node> parseFactor(const std::vector<std::string>& tokens, size_t& index);
    
    // Вспомогательные методы
    bool isNumber(const std::string& token) const;
    bool isOperator(const std::string& token) const;
};