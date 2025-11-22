#pragma once
#include "Node.h"

// Узел для числовых литералов
class NumberNode : public Node {
private:
    double value_;

public:
    explicit NumberNode(double value);
    
    double evaluate() const override;
    
    double getValue() const;
};