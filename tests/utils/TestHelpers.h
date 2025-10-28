#pragma once
#include <cmath>
#include <string>

namespace TestHelpers {
    constexpr double EPSILON = 1e-10;
    
    inline bool areEqual(double a, double b) {
        return std::abs(a - b) < EPSILON;
    }
    
    inline std::string createTempPluginDir() {
        return "./test_plugins_temp/";
    }
    
    class TestRAII {
    public:
        TestRAII() = default;
        ~TestRAII() {
            // Здесь будет очистка временных файлов
        }
    };
}