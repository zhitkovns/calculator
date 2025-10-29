#include "../test_framework.h"
#include "../../src/core/Calculator.h"

void runCalculatorTests(TestFramework& tf) {
    std::cout << "\n=== Calculator Tests ===" << std::endl;
    
    std::cout << "TEST: Creation and initialization ... ";
    try {
        Calculator calculator;
        calculator.initialize();
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Basic operations ... ";
    try {
        Calculator calculator;
        calculator.initialize();
        tf.assertDoubleEqual(calculator.calculate("2 + 3"), 5.0);
        tf.assertDoubleEqual(calculator.calculate("10 - 4"), 6.0);
        tf.assertDoubleEqual(calculator.calculate("3 * 4"), 12.0);
        tf.assertDoubleEqual(calculator.calculate("15 / 3"), 5.0);
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Complex expressions ... ";
    try {
        Calculator calculator;
        calculator.initialize();
        tf.assertDoubleEqual(calculator.calculate("2 + 3 * 4"), 14.0);
        tf.assertDoubleEqual(calculator.calculate("(2 + 3) * 4"), 20.0);
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Available operations ... ";
    try {
        Calculator calculator;
        calculator.initialize();
        auto operations = calculator.getAvailableOperations();
        tf.assertTrue(calculator.hasOperation("+"));
        tf.assertTrue(calculator.hasOperation("-"));
        tf.assertTrue(calculator.hasOperation("*"));
        tf.assertTrue(calculator.hasOperation("/"));
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Division by zero ... ";
    try {
        Calculator calculator;
        calculator.initialize();
        tf.assertThrows([&]() { calculator.calculate("10 / 0"); });
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Invalid expressions ... ";
    try {
        Calculator calculator;
        calculator.initialize();
        tf.assertThrows([&]() { calculator.calculate("2 + + 3"); });
        tf.assertThrows([&]() { calculator.calculate(""); });
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
}