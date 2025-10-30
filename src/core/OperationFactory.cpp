#include "OperationFactory.h"
#include <stdexcept>
#include <algorithm>

void OperationFactory::registerOperation(const std::string& name, IOperation* operation) {
    if (!operation) {
        throw std::invalid_argument("Cannot register null operation");
    }
    
    if (name.empty()) {
        throw std::invalid_argument("Operation name cannot be empty");
    }
    
    if (operations_.find(name) != operations_.end()) {
        throw std::runtime_error("Operation '" + name + "' is already registered");
    }
    
    operations_[name] = operation;
}

IOperation* OperationFactory::getOperation(const std::string& name) const {
    auto it = operations_.find(name);
    return (it != operations_.end()) ? it->second : nullptr;
}

bool OperationFactory::hasOperation(const std::string& name) const {
    return operations_.find(name) != operations_.end();
}

std::vector<std::string> OperationFactory::getAvailableOperations() const {
    std::vector<std::string> result;
    result.reserve(operations_.size()); // Оптимизация: резервируем память заранее
    
    for (const auto& pair : operations_) {
        result.push_back(pair.first);
    }
    
    // Сортируем для удобства пользователя
    std::sort(result.begin(), result.end());
    return result;
}

void OperationFactory::clear() {
    operations_.clear();
}