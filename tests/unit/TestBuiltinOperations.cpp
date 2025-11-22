#include "../test_framework.h"
#include "../../src/operations/BuiltinOperations.h"

void runBuiltinOperationsTests(TestFramework& tf) {
    std::cout << "\n=== BuiltinOperations Tests ===" << std::endl;
    
    std::cout << "TEST: Add operation ... ";
    try {
        AddOperation op;
        tf.assertEqual(op.getName(), std::string("+"));
        tf.assertTrue(op.getType() == OperationType::BINARY, "Operation type should be BINARY");
        tf.assertEqual(op.getPriority(), 1);
        tf.assertEqual(op.getArgumentCount(), (size_t)2);
        tf.assertFalse(op.isRightAssociative());
        
        std::vector<double> args = {2.0, 3.0};
        tf.assertDoubleEqual(op.execute(args), 5.0);
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Subtract operation ... ";
    try {
        SubtractOperation op;
        tf.assertEqual(op.getName(), std::string("-"));
        
        std::vector<double> args = {10.0, 4.0};
        tf.assertDoubleEqual(op.execute(args), 6.0);
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Multiply operation ... ";
    try {
        MultiplyOperation op;
        tf.assertEqual(op.getName(), std::string("*"));
        tf.assertEqual(op.getPriority(), 2);
        
        std::vector<double> args = {3.0, 4.0};
        tf.assertDoubleEqual(op.execute(args), 12.0);
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Divide operation ... ";
    try {
        DivideOperation op;
        tf.assertEqual(op.getName(), std::string("/"));
        
        std::vector<double> args = {15.0, 3.0};
        tf.assertDoubleEqual(op.execute(args), 5.0);
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST: Division by zero ... ";
    try {
        DivideOperation op;
        std::vector<double> args = {10.0, 0.0};
        tf.assertThrows([&]() { op.execute(args); });
        std::cout << "PASS" << std::endl;
    } catch (...) {
        std::cout << "FAIL" << std::endl;
    }
}