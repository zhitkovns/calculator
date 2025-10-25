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
    return OperationType::FUNCTION;
}

int ExtensionOperationWrapper::getPriority() const {
    return 4; // Высокий приоритет для функций
}

size_t ExtensionOperationWrapper::getArgumentCount() const {
    if (!extension_) return 0;
    
    auto range = extension_->getParameterRange();
    return range.first; // Минимальное количество параметров
}