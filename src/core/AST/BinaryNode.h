#pragma once
#include "Node.h"
#include "operations/IOperation.h"
#include <memory>

// Узел для бинарных операций (+, -, *, /, ^)
class BinaryNode : public Node {
private:
    std::unique_ptr<Node> left_;
    std::unique_ptr<Node> right_;
    IOperation* operation_;  // Не владеем операцией, она принадлежит фабрике

public:
    BinaryNode(std::unique_ptr<Node> left, std::unique_ptr<Node> right, IOperation* operation);
    
    double evaluate() const override;
    
    const Node* getLeft() const;
    const Node* getRight() const;
    IOperation* getOperation() const;
};