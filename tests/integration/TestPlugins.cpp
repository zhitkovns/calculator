#include <gtest/gtest.h>
#include "../../src/core/Calculator.h"
#include <filesystem>

class PluginTest : public ::testing::Test {
protected:
    void SetUp() override {
        calculator = std::make_unique<Calculator>();
        calculator->initialize();
    }
    
    std::unique_ptr<Calculator> calculator;
};

TEST_F(PluginTest, PluginOperationsAvailable) {
    auto operations = calculator->getAvailableOperations();
    
    // Проверяем что плагинные операции доступны
    EXPECT_TRUE(calculator->hasOperation("sin"));
    EXPECT_TRUE(calculator->hasOperation("cos"));
    EXPECT_TRUE(calculator->hasOperation("ln"));
}

TEST_F(PluginTest, PluginOperationsCalculation) {
    // Проверяем вычисления через плагины
    EXPECT_NO_THROW(calculator->calculate("sin(0)"));
    EXPECT_NO_THROW(calculator->calculate("cos(0)"));
    EXPECT_NO_THROW(calculator->calculate("ln(1)"));
}

TEST_F(PluginTest, ComplexExpressionWithPlugins) {
    // Комбинируем базовые операции и плагины
    EXPECT_NO_THROW(calculator->calculate("2 + sin(0) * 3"));
    EXPECT_NO_THROW(calculator->calculate("(cos(0) + 1) * 2"));
}