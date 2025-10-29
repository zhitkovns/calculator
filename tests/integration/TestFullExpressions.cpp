#include "../test_framework.h"
#include "../../src/core/Calculator.h"

void runFullExpressionsTests(TestFramework& tf) {
    std::cout << "\n=== FullExpressions Tests ===" << std::endl;
    
    std::cout << "TEST: Complex mathematical expression ... ";
    try {
        Calculator calculator;
        calculator.initialize();
        double result = calculator.calculate("(10 + 2) * 3 - 8 / 2");
        tf.assertDoubleEqual(result, 32.0);
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Nested parentheses ... ";
    try {
        Calculator calculator;
        calculator.initialize();
        double result = calculator.calculate("((2 + 3) * (4 - 1)) + 10");
        tf.assertDoubleEqual(result, 25.0);
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Operator precedence with mixed operations ... ";
    try {
        Calculator calculator;
        calculator.initialize();
        double result1 = calculator.calculate("2 + 3 * 4 - 6 / 2");
        double result2 = calculator.calculate("2 + (3 * 4) - (6 / 2)");
        tf.assertDoubleEqual(result1, result2);
        tf.assertDoubleEqual(result1, 11.0);
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Complex expression with all operations ... ";
    try {
        Calculator calculator;
        calculator.initialize();
        double result = calculator.calculate("(2.5 + 3.5) * (10 - 4) / 3 + 1");
        tf.assertDoubleEqual(result, 13.0);
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
}