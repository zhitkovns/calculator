#include <gtest/gtest.h>
#include "../../src/core/ExpressionParser.h"
#include "../../src/core/OperationFactory.h"
#include "../../src/operations/BuiltinOperations.h"

using namespace testing;

class ExpressionParserTest : public Test {
protected:
    void SetUp() override {
        factory = std::make_unique<OperationFactory>();
        
        // Регистрируем реальные операции с правильными именами
        factory->registerOperation("+", new AddOperation());
        factory->registerOperation("-", new SubtractOperation());
        factory->registerOperation("*", new MultiplyOperation());
        factory->registerOperation("/", new DivideOperation());
        
        parser = std::make_unique<ExpressionParser>(*factory);
    }
    
    void TearDown() override {
        // Фабрика владеет операциями, они удалятся автоматически
    }
    
    std::unique_ptr<OperationFactory> factory;
    std::unique_ptr<ExpressionParser> parser;
};

TEST_F(ExpressionParserTest, SimpleAddition) {
    auto ast = parser->parse("2 + 3");
    EXPECT_DOUBLE_EQ(ast->evaluate(), 5.0);
}

TEST_F(ExpressionParserTest, OperatorPrecedence) {
    auto ast = parser->parse("2 + 3 * 4");
    EXPECT_DOUBLE_EQ(ast->evaluate(), 14.0); // 2 + (3*4) = 14
}

TEST_F(ExpressionParserTest, Parentheses) {
    auto ast = parser->parse("(2 + 3) * 4");
    EXPECT_DOUBLE_EQ(ast->evaluate(), 20.0);
}

TEST_F(ExpressionParserTest, ComplexExpression) {
    auto ast = parser->parse("(10 - 2) * (3 + 4) / 2");
    EXPECT_DOUBLE_EQ(ast->evaluate(), 28.0); // 8 * 7 / 2 = 28
}

TEST_F(ExpressionParserTest, InvalidExpressionThrows) {
    EXPECT_THROW(parser->parse("2 + + 3"), std::runtime_error);
    EXPECT_THROW(parser->parse("(2 + 3"), std::runtime_error);
    EXPECT_THROW(parser->parse("2 + 3)"), std::runtime_error);
}

TEST_F(ExpressionParserTest, FloatingPointNumbers) {
    auto ast = parser->parse("2.5 + 3.7");
    EXPECT_DOUBLE_EQ(ast->evaluate(), 6.2);
}

TEST_F(ExpressionParserTest, NegativeNumbers) {
    auto ast = parser->parse("5 - 7"); // 5 - 7 = -2
    EXPECT_DOUBLE_EQ(ast->evaluate(), -2.0);
    auto ast2 = parser->parse("(5 - 10) * 2"); // (-5) * 2 = -10
    EXPECT_DOUBLE_EQ(ast2->evaluate(), -10.0);
}

TEST_F(ExpressionParserTest, MultipleSpaces) {
    auto ast = parser->parse("  2   +   3   *   4  ");
    EXPECT_DOUBLE_EQ(ast->evaluate(), 14.0);
}

TEST_F(ExpressionParserTest, DeeplyNestedParentheses) {
    auto ast = parser->parse("((((2 + 3))))");
    EXPECT_DOUBLE_EQ(ast->evaluate(), 5.0);
}