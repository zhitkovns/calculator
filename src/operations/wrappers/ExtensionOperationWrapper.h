#pragma once
#include "../IOperation.h"
#include "../../plugins/ExtensionUnit.h"
#include <memory>

class ExtensionOperationWrapper : public IOperation {
private:
    std::shared_ptr<ExtensionUnit> extension_;
    std::string functionName_;

public:
    ExtensionOperationWrapper(std::shared_ptr<ExtensionUnit> extension, const std::string& functionName);
    
    double execute(const std::vector<double>& args) override;
    std::string getName() const override;
    OperationType getType() const override;
    int getPriority() const override;
    size_t getArgumentCount() const override;
};