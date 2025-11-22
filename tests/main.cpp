#include "test_framework.h"

int main() {
    TestFramework tf;
    
    std::cout << "Running Calculator tests...\n" << std::endl;
    
    // Запускаем все тесты
    runCalculatorTests(tf);
    runExpressionParserTests(tf);
    runASTTests(tf);
    runBuiltinOperationsTests(tf);
    runErrorHandlingTests(tf);
    runFullExpressionsTests(tf);
    runPluginTests(tf);
    
    tf.printSummary();
    
    return tf.getTestsFailed() > 0 ? 1 : 0;
}