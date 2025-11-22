#pragma once
#include <memory>

// Базовый класс для всех узлов AST
class Node {
public:
    virtual ~Node() = default;
    
    // Вычисление значения узла
    virtual double evaluate() const = 0;
};

// Умный указатель для узлов
using NodePtr = std::unique_ptr<Node>;