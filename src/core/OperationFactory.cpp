#include "OperationFactory.h"
#include <stdexcept>

void OperationFactory::registerOperation(const std::string& name, IOperation* operation) {
    if (!operation) {
        throw std::invalid_argument("Cannot register null operation");
    }
    
    if (operations_.find(name) != operations_.end()) {
        throw std::runtime_error("Operation '" + name + "' is already registered");
    }
    
    operations_[name] = operation;
}

IOperation* OperationFactory::getOperation(const std::string& name) const {
    auto it = operations_.find(name);
    if (it != operations_.end()) {
        return it->second;
    }
    return nullptr;
}

bool OperationFactory::hasOperation(const std::string& name) const {
    return operations_.find(name) != operations_.end();
}

std::vector<std::string> OperationFactory::getAvailableOperations() const {
    std::vector<std::string> result;
    for (const auto& pair : operations_) {
        result.push_back(pair.first);
    }
    return result;
}

void OperationFactory::clear() {
    operations_.clear();
}