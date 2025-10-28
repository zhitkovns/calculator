#include <gtest/gtest.h>
#include "../../src/core/AST/NumberNode.h"
#include "../../src/core/AST/BinaryNode.h"
#include "../../src/core/AST/UnaryNode.h"
#include "../../src/core/AST/FunctionNode.h"
#include "../mocks/MockOperation.h"

TEST(ASTTest, NumberNodeEvaluation) {
    NumberNode node(42.5);
    EXPECT_DOUBLE_EQ(node.evaluate(), 42.5);
}

TEST(ASTTest, BinaryNodeEvaluation) {
    auto left = std::make_unique<NumberNode>(10.0);
    auto right = std::make_unique<NumberNode>(5.0);
    
    auto mockOperation = std::make_unique<MockOperation>();
    mockOperation->execute_callback = [](const std::vector<double>& args) {
        return args[0] + args[1]; // 10 + 5 = 15
    };
    mockOperation->argCount = 2;
    
    BinaryNode node(std::move(left), std::move(right), mockOperation.get());
    EXPECT_DOUBLE_EQ(node.evaluate(), 15.0);
}

TEST(ASTTest, UnaryNodeEvaluation) {
    auto child = std::make_unique<NumberNode>(5.0);
    
    auto mockOperation = std::make_unique<MockOperation>();
    mockOperation->execute_callback = [](const std::vector<double>& args) {
        return -args[0]; // унарный минус
    };
    mockOperation->argCount = 1;
    mockOperation->type = OperationType::UNARY;
    
    UnaryNode node(std::move(child), mockOperation.get());
    EXPECT_DOUBLE_EQ(node.evaluate(), -5.0);
}

TEST(ASTTest, FunctionNodeEvaluation) {
    std::vector<std::unique_ptr<Node>> args;
    args.push_back(std::make_unique<NumberNode>(2.0));
    
    auto mockOperation = std::make_unique<MockOperation>();
    mockOperation->execute_callback = [](const std::vector<double>& args) {
        return args[0] * args[0]; // квадрат
    };
    mockOperation->argCount = 1;
    
    FunctionNode node(std::move(args), mockOperation.get());
    EXPECT_DOUBLE_EQ(node.evaluate(), 4.0);
}