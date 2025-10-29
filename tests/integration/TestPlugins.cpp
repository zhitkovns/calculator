#include "../test_framework.h"
#include "../../src/core/Calculator.h"
#include <cmath>

void runPluginTests(TestFramework& tf) {
    std::cout << "\n=== Plugin Tests ===" << std::endl;
    
    std::cout << "TEST: Plugin operations available ... ";
    try {
        Calculator calculator;
        calculator.initialize();
        auto operations = calculator.getAvailableOperations();
        tf.assertTrue(calculator.hasOperation("sin"));
        tf.assertTrue(calculator.hasOperation("cos"));
        tf.assertTrue(calculator.hasOperation("ln"));
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Plugin operations calculation ... ";
    try {
        Calculator calculator;
        calculator.initialize();
        
        tf.assertDoubleEqual(calculator.calculate("sin(0)"), 0.0);
        tf.assertDoubleEqual(calculator.calculate("cos(0)"), 1.0);
        tf.assertDoubleEqual(calculator.calculate("ln(1)"), 0.0);
        
        tf.assertThrows([&]() { calculator.calculate("ln(-1)"); });  // ln от отрицательного числа
        tf.assertThrows([&]() { calculator.calculate("sin()"); });   // нет аргументов
        tf.assertThrows([&]() { calculator.calculate("cos(1, 2)"); }); // слишком много аргументов
        
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Complex expression with plugins ... ";
    try {
        Calculator calculator;
        calculator.initialize();
        
        tf.assertDoubleEqual(calculator.calculate("2 + sin(0) * 3"), 2.0);
        tf.assertDoubleEqual(calculator.calculate("(cos(0) + 1) * 2"), 4.0);
        
        tf.assertThrows([&]() { calculator.calculate("sin(0) + unknown(5)"); }); // неизвестная функция
        tf.assertThrows([&]() { calculator.calculate("ln(0)"); }); // ln(0) - математическая ошибка
        
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
}