#include <gtest/gtest.h>
#include "../../src/core/Calculator.h"

TEST(CalculatorTest, CreationAndInitialization) {
    Calculator calculator;
    EXPECT_NO_THROW(calculator.initialize());
}

TEST(CalculatorTest, BasicOperations) {
    Calculator calculator;
    calculator.initialize();
    
    EXPECT_DOUBLE_EQ(calculator.calculate("2 + 3"), 5.0);
    EXPECT_DOUBLE_EQ(calculator.calculate("10 - 4"), 6.0);
    EXPECT_DOUBLE_EQ(calculator.calculate("3 * 4"), 12.0);
    EXPECT_DOUBLE_EQ(calculator.calculate("15 / 3"), 5.0);
}

TEST(CalculatorTest, ComplexExpressions) {
    Calculator calculator;
    calculator.initialize();
    
    EXPECT_DOUBLE_EQ(calculator.calculate("2 + 3 * 4"), 14.0);
    EXPECT_DOUBLE_EQ(calculator.calculate("(2 + 3) * 4"), 20.0);
}

TEST(CalculatorTest, AvailableOperations) {
    Calculator calculator;
    calculator.initialize();
    
    auto operations = calculator.getAvailableOperations();
    // Должны быть базовые операции
    EXPECT_TRUE(calculator.hasOperation("+"));
    EXPECT_TRUE(calculator.hasOperation("-"));
    EXPECT_TRUE(calculator.hasOperation("*"));
    EXPECT_TRUE(calculator.hasOperation("/"));
}

TEST(CalculatorTest, DivisionByZero) {
    Calculator calculator;
    calculator.initialize();
    
    EXPECT_THROW(calculator.calculate("10 / 0"), std::runtime_error);
}

TEST(CalculatorTest, InvalidExpression) {
    Calculator calculator;
    calculator.initialize();
    
    EXPECT_THROW(calculator.calculate("2 + + 3"), std::runtime_error);
    EXPECT_THROW(calculator.calculate(""), std::invalid_argument);
    EXPECT_THROW(calculator.calculate("sin()"), std::runtime_error);
}