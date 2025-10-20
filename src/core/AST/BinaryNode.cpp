#include "BinaryNode.h"
#include <stdexcept>

BinaryNode::BinaryNode(std::unique_ptr<Node> left, std::unique_ptr<Node> right, IOperation* operation)
    : left_(std::move(left)), right_(std::move(right)), operation_(operation) {
    
    if (!operation_) {
        throw std::invalid_argument("BinaryNode: operation cannot be null");
    }
    if (operation_->getType() != OperationType::BINARY) {
        throw std::invalid_argument("BinaryNode: operation must be binary");
    }
}

double BinaryNode::evaluate() const {
    if (!left_ || !right_) {
        throw std::runtime_error("BinaryNode: missing left or right operand");
    }
    
    double leftValue = left_->evaluate();
    double rightValue = right_->evaluate();
    
    std::vector<double> args = {leftValue, rightValue};
    return operation_->execute(args);
}

const Node* BinaryNode::getLeft() const {
    return left_.get();
}

const Node* BinaryNode::getRight() const {
    return right_.get();
}

IOperation* BinaryNode::getOperation() const {
    return operation_;
}