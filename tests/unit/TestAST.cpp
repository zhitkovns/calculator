#include "../test_framework.h"
#include "../../src/core/AST/NumberNode.h"
#include "../../src/core/AST/BinaryNode.h"
#include "../../src/core/AST/UnaryNode.h"
#include "../../src/core/AST/FunctionNode.h"

void runASTTests(TestFramework& tf) {
    std::cout << "\n=== AST Tests ===" << std::endl;
    
    std::cout << "TEST: Number node evaluation ... ";
    try {
        NumberNode node(42.5);
        tf.assertDoubleEqual(node.evaluate(), 42.5);
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Binary node evaluation ... ";
    try {
        auto left = std::make_unique<NumberNode>(10.0);
        auto right = std::make_unique<NumberNode>(5.0);
        auto mockOperation = std::make_unique<MockOperation>();
        mockOperation->execute_callback = [](const std::vector<double>& args) {
            return args[0] + args[1];
        };
        mockOperation->argCount = 2;
        
        BinaryNode node(std::move(left), std::move(right), mockOperation.get());
        tf.assertDoubleEqual(node.evaluate(), 15.0);
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Unary node evaluation ... ";
    try {
        auto child = std::make_unique<NumberNode>(5.0);
        auto mockOperation = std::make_unique<MockOperation>();
        mockOperation->execute_callback = [](const std::vector<double>& args) {
            return -args[0];
        };
        mockOperation->argCount = 1;
        mockOperation->type = OperationType::UNARY;
        
        UnaryNode node(std::move(child), mockOperation.get());
        tf.assertDoubleEqual(node.evaluate(), -5.0);
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Function node evaluation ... ";
    try {
        std::vector<std::unique_ptr<Node>> args;
        args.push_back(std::make_unique<NumberNode>(2.0));
        auto mockOperation = std::make_unique<MockOperation>();
        mockOperation->execute_callback = [](const std::vector<double>& args) {
            return args[0] * args[0];
        };
        mockOperation->argCount = 1;
        
        FunctionNode node(std::move(args), mockOperation.get());
        tf.assertDoubleEqual(node.evaluate(), 4.0);
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
}