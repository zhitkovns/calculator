#include <gtest/gtest.h>
#include "../../src/core/Calculator.h"

TEST(ErrorHandlingTest, InvalidCharacters) {
    Calculator calculator;
    calculator.initialize();
    
    EXPECT_THROW(calculator.calculate("2 @ 3"), std::runtime_error);
    EXPECT_THROW(calculator.calculate("abc + 3"), std::runtime_error);
}

TEST(ErrorHandlingTest, UnbalancedParentheses) {
    Calculator calculator;
    calculator.initialize();
    
    EXPECT_THROW(calculator.calculate("(2 + 3"), std::runtime_error);
    EXPECT_THROW(calculator.calculate("2 + 3)"), std::runtime_error);
    EXPECT_THROW(calculator.calculate("((2 + 3)"), std::runtime_error);
}

TEST(ErrorHandlingTest, InvalidFunctionCalls) {
    Calculator calculator;
    calculator.initialize();
    
    EXPECT_THROW(calculator.calculate("sin()"), std::runtime_error);
    EXPECT_THROW(calculator.calculate("sin(1, 2)"), std::runtime_error);
}