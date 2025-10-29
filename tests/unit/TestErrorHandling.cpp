#include "../test_framework.h"
#include "../../src/core/Calculator.h"

void runErrorHandlingTests(TestFramework& tf) {
    std::cout << "\n=== ErrorHandling Tests ===" << std::endl;
    
    std::cout << "TEST: Invalid characters ... ";
    try {
        Calculator calculator;
        calculator.initialize();
        tf.assertThrows([&]() { calculator.calculate("2 @ 3"); });
        tf.assertThrows([&]() { calculator.calculate("abc + 3"); });
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Unbalanced parentheses ... ";
    try {
        Calculator calculator;
        calculator.initialize();
        tf.assertThrows([&]() { calculator.calculate("(2 + 3"); });
        tf.assertThrows([&]() { calculator.calculate("2 + 3)"); });
        tf.assertThrows([&]() { calculator.calculate("((2 + 3)"); });
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Invalid function calls ... ";
    try {
        Calculator calculator;
        calculator.initialize();
        tf.assertThrows([&]() { calculator.calculate("sin()"); });
        tf.assertThrows([&]() { calculator.calculate("sin(1, 2)"); });
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
}