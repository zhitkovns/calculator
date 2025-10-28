#include <gtest/gtest.h>
#include "../../src/operations/BuiltinOperations.h"

TEST(BuiltinOperationsTest, AddOperation) {
    AddOperation op;
    EXPECT_EQ(op.getName(), "+");
    EXPECT_EQ(op.getType(), OperationType::BINARY);
    EXPECT_EQ(op.getPriority(), 1);
    EXPECT_EQ(op.getArgumentCount(), 2);
    EXPECT_FALSE(op.isRightAssociative());
    
    std::vector<double> args = {2.0, 3.0};
    EXPECT_DOUBLE_EQ(op.execute(args), 5.0);
}

TEST(BuiltinOperationsTest, SubtractOperation) {
    SubtractOperation op;
    EXPECT_EQ(op.getName(), "-");
    
    std::vector<double> args = {10.0, 4.0};
    EXPECT_DOUBLE_EQ(op.execute(args), 6.0);
}

TEST(BuiltinOperationsTest, MultiplyOperation) {
    MultiplyOperation op;
    EXPECT_EQ(op.getName(), "*");
    EXPECT_EQ(op.getPriority(), 2);
    
    std::vector<double> args = {3.0, 4.0};
    EXPECT_DOUBLE_EQ(op.execute(args), 12.0);
}

TEST(BuiltinOperationsTest, DivideOperation) {
    DivideOperation op;
    EXPECT_EQ(op.getName(), "/");
    
    std::vector<double> args = {15.0, 3.0};
    EXPECT_DOUBLE_EQ(op.execute(args), 5.0);
}

TEST(BuiltinOperationsTest, DivisionByZero) {
    DivideOperation op;
    std::vector<double> args = {10.0, 0.0};
    EXPECT_THROW(op.execute(args), std::runtime_error);
}