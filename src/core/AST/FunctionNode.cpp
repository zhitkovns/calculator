#include "FunctionNode.h"
#include <stdexcept>

FunctionNode::FunctionNode(std::vector<std::unique_ptr<Node>> arguments, IOperation* function)
    : arguments_(std::move(arguments)), function_(function) {
    
    if (!function_) {
        throw std::invalid_argument("Function cannot be null");
    }
}

double FunctionNode::evaluate() const {
    std::vector<double> args;
    for (const auto& arg : arguments_) {
        args.push_back(arg->evaluate());
    }
    
    return function_->execute(args);
}