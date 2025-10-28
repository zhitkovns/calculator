#include <gtest/gtest.h>
#include "../../src/core/Calculator.h"

class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        calculator = std::make_unique<Calculator>();
        calculator->initialize();
    }
    
    std::unique_ptr<Calculator> calculator;
};

TEST_F(IntegrationTest, ComplexMathematicalExpression) {
    double result = calculator->calculate("(10 + 2) * 3 - 8 / 2");
    EXPECT_DOUBLE_EQ(result, 32.0); // 12 * 3 - 4 = 32
}

TEST_F(IntegrationTest, NestedParentheses) {
    double result = calculator->calculate("((2 + 3) * (4 - 1)) + 10");
    EXPECT_DOUBLE_EQ(result, 25.0); // (5 * 3) + 10 = 25
}

TEST_F(IntegrationTest, OperatorPrecedenceWithMixedOperations) {
    double result1 = calculator->calculate("2 + 3 * 4 - 6 / 2");
    double result2 = calculator->calculate("2 + (3 * 4) - (6 / 2)");
    EXPECT_DOUBLE_EQ(result1, result2); // Должны быть равны
    EXPECT_DOUBLE_EQ(result1, 11.0); // 2 + 12 - 3 = 11
}