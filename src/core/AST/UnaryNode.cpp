#include "UnaryNode.h"
#include <stdexcept>

UnaryNode::UnaryNode(std::unique_ptr<Node> argument, IOperation* operation)
    : argument_(std::move(argument)), operation_(operation) {
    
    if (!operation_) {
        throw std::invalid_argument("UnaryNode: operation cannot be null");
    }
    if (operation_->getType() != OperationType::UNARY && operation_->getType() != OperationType::FUNCTION) {
        throw std::invalid_argument("UnaryNode: operation must be unary or function");
    }
}

double UnaryNode::evaluate() const {
    if (!argument_) {
        throw std::runtime_error("UnaryNode: missing argument");
    }
    
    double argValue = argument_->evaluate();
    std::vector<double> args = {argValue};
    return operation_->execute(args);
}

const Node* UnaryNode::getArgument() const {
    return argument_.get();
}

IOperation* UnaryNode::getOperation() const {
    return operation_;
}