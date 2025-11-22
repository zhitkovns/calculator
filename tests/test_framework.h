#pragma once
#include <iostream>
#include <string>
#include <cmath>
#include <functional>
#include <memory>
#include <vector>
#include "../src/operations/IOperation.h"


// Test Framework
class TestFramework {
private:
    int testsPassed = 0;
    int testsFailed = 0;
    
public:
    template<typename T>
    void assertEqual(const T& actual, const T& expected, const std::string& message = "") {
        if (actual != expected) {
            std::cout << "FAIL - Expected: " << expected << ", Got: " << actual;
            if (!message.empty()) std::cout << " (" << message << ")";
            std::cout << std::endl;
            testsFailed++;
            throw std::runtime_error("Assertion failed");
        }
        testsPassed++;
    }
    
    void assertTrue(bool condition, const std::string& message = "") {
        if (!condition) {
            std::cout << "FAIL - Condition is false";
            if (!message.empty()) std::cout << " (" << message << ")";
            std::cout << std::endl;
            testsFailed++;
            throw std::runtime_error("Assertion failed");
        }
        testsPassed++;
    }
    
    void assertFalse(bool condition, const std::string& message = "") {
        assertTrue(!condition, message);
    }
    
    void assertThrows(std::function<void()> func, const std::string& expectedError = "") {
        try {
            func();
            std::cout << "FAIL - Expected exception but none was thrown";
            if (!expectedError.empty()) std::cout << " (" << expectedError << ")";
            std::cout << std::endl;
            testsFailed++;
        } catch (const std::exception& e) {
            testsPassed++;
        }
    }
    
    void assertDoubleEqual(double actual, double expected, double epsilon = 1e-10, const std::string& message = "") {
        if (std::abs(actual - expected) > epsilon) {
            std::cout << "FAIL - Expected: " << expected << ", Got: " << actual;
            if (!message.empty()) std::cout << " (" << message << ")";
            std::cout << std::endl;
            testsFailed++;
            throw std::runtime_error("Assertion failed");
        }
        testsPassed++;
    }
    
    void printSummary() {
        std::cout << "\n=== TEST SUMMARY ===" << std::endl;
        std::cout << "Passed: " << testsPassed << std::endl;
        std::cout << "Failed: " << testsFailed << std::endl;
        std::cout << "Total:  " << (testsPassed + testsFailed) << std::endl;
        
        std::cout << (testsFailed == 0 ? "ALL TESTS PASSED!" : "SOME TESTS FAILED!") << std::endl;
    }
    
    int getTestsFailed() const { return testsFailed; }
};

// Mock для операций
class MockOperation : public IOperation {
public:
    virtual ~MockOperation() = default;
    
    virtual double execute(const std::vector<double>& args) override {
        return execute_callback ? execute_callback(args) : 0.0;
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

// Объявления тестовых функций
void runCalculatorTests(TestFramework& tf);
void runExpressionParserTests(TestFramework& tf);
void runASTTests(TestFramework& tf);
void runBuiltinOperationsTests(TestFramework& tf);
void runErrorHandlingTests(TestFramework& tf);
void runFullExpressionsTests(TestFramework& tf);
void runPluginTests(TestFramework& tf);