#include <gtest/gtest.h>
#include <iostream>

int main(int argc, char** argv) {
    std::cout << "Running Calculator tests..." << std::endl;
    
    ::testing::InitGoogleTest(&argc, argv);
    
    int result = RUN_ALL_TESTS();
    
    std::cout << "Tests completed with result: " << result << std::endl;
    return result;
}