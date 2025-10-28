#pragma once
#include "../../src/operations/IOperation.h"
#include <gtest/gtest.h>

class MockOperation : public IOperation {
public:
    virtual ~MockOperation() = default;
    
    // Простые реализации вместо gmock
    virtual double execute(const std::vector<double>& args) override {
        if (execute_callback) {
            return execute_callback(args);
        }
        return 0.0;
    }
    
    virtual std::string getName() const override { 
        return name; 
    }
    
    virtual OperationType getType() const override { 
        return type; 
    }
    
    virtual int getPriority() const override { 
        return priority; 
    }
    
    virtual size_t getArgumentCount() const override { 
        return argCount; 
    }
    
    virtual bool isRightAssociative() const override { 
        return rightAssociative; 
    }
    
    // Поля для настройки мока
    std::function<double(const std::vector<double>&)> execute_callback;
    std::string name = "mock";
    OperationType type = OperationType::BINARY;
    int priority = 1;
    size_t argCount = 2;
    bool rightAssociative = false;
};