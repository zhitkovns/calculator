#pragma once
#include "Node.h"
#include "operations/IOperation.h"
#include <memory>

// Узел для унарных операций и функций (sin, cos, унарный минус и т.д.)
class UnaryNode : public Node {
private:
    std::unique_ptr<Node> argument_;
    IOperation* operation_;  // Не владеем операцией

public:
    UnaryNode(std::unique_ptr<Node> argument, IOperation* operation);
    
    double evaluate() const override;
    
    const Node* getArgument() const;
    IOperation* getOperation() const;
};