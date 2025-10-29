#include "../test_framework.h"
#include "../../src/core/ExpressionParser.h"
#include "../../src/core/OperationFactory.h"
#include "../../src/operations/BuiltinOperations.h"

void runExpressionParserTests(TestFramework& tf) {
    std::cout << "\n=== ExpressionParser Tests ===" << std::endl;
    
    auto factory = std::make_unique<OperationFactory>();
    factory->registerOperation("+", new AddOperation());
    factory->registerOperation("-", new SubtractOperation());
    factory->registerOperation("*", new MultiplyOperation());
    factory->registerOperation("/", new DivideOperation());
    auto parser = std::make_unique<ExpressionParser>(*factory);
    
    std::cout << "TEST: Simple addition ... ";
    try {
        auto ast = parser->parse("2 + 3");
        tf.assertDoubleEqual(ast->evaluate(), 5.0);
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Operator precedence ... ";
    try {
        auto ast = parser->parse("2 + 3 * 4");
        tf.assertDoubleEqual(ast->evaluate(), 14.0);
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Parentheses ... ";
    try {
        auto ast = parser->parse("(2 + 3) * 4");
        tf.assertDoubleEqual(ast->evaluate(), 20.0);
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Complex expression ... ";
    try {
        auto ast = parser->parse("(10 - 2) * (3 + 4) / 2");
        tf.assertDoubleEqual(ast->evaluate(), 28.0);
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Invalid expressions throw ... ";
    try {
        tf.assertThrows([&]() { parser->parse("2 + + 3"); });
        tf.assertThrows([&]() { parser->parse("(2 + 3"); });
        tf.assertThrows([&]() { parser->parse("2 + 3)"); });
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Floating point numbers ... ";
    try {
        auto ast = parser->parse("2.5 + 3.7");
        tf.assertDoubleEqual(ast->evaluate(), 6.2);
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Negative results ... ";
    try {
        auto ast = parser->parse("5 - 7");
        tf.assertDoubleEqual(ast->evaluate(), -2.0);
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Multiple spaces ... ";
    try {
        auto ast = parser->parse("  2   +   3   *   4  ");
        tf.assertDoubleEqual(ast->evaluate(), 14.0);
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Deeply nested parentheses ... ";
    try {
        auto ast = parser->parse("((((2 + 3))))");
        tf.assertDoubleEqual(ast->evaluate(), 5.0);
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
}