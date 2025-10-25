#pragma once
#include "Node.h"
#include "operations/IOperation.h"
#include <memory>
#include <vector>

class FunctionNode : public Node {
private:
    std::vector<std::unique_ptr<Node>> arguments_;
    IOperation* function_;

public:
    FunctionNode(std::vector<std::unique_ptr<Node>> arguments, IOperation* function);
    double evaluate() const override;
};