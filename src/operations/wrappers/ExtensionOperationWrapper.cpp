#include "ExtensionOperationWrapper.h"

ExtensionOperationWrapper::ExtensionOperationWrapper(std::shared_ptr<ExtensionUnit> extension, const std::string& functionName)
    : extension_(extension), functionName_(functionName) {
}

double ExtensionOperationWrapper::execute(const std::vector<double>& args) {
    if (!extension_) {
        throw std::runtime_error("Extension not available for function: " + functionName_);
    }
    
    std::string error_msg;
    double result = extension_->compute(args, &error_msg);
    
    if (!error_msg.empty()) {
        throw std::runtime_error("Extension error in " + functionName_ + ": " + error_msg);
    }
    
    return result;
}

std::string ExtensionOperationWrapper::getName() const {
    return functionName_;
}

OperationType ExtensionOperationWrapper::getType() const {
    OperationCategory category = extension_->getCategory();
    switch (category) {
        case OperationCategory::TWO_OPERAND:
            return OperationType::BINARY;
        case OperationCategory::SINGLE_OPERAND:
            return OperationType::UNARY;
        case OperationCategory::MATH_FUNCTION:
        default:
            return OperationType::FUNCTION;
    }
}

int ExtensionOperationWrapper::getPriority() const {
    return extension_->getPriorityValue();
}

size_t ExtensionOperationWrapper::getArgumentCount() const {
    if (!extension_) return 0;
    
    auto range = extension_->getParameterRange();
    return range.first;
}

bool ExtensionOperationWrapper::isRightAssociative() const {
    return extension_->isRightAssociative();
}