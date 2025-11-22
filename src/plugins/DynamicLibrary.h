#pragma once
#include <string>
#include <memory>
#include <windows.h>

// Класс-обертка для динамической загрузки DLL
class DynamicLibrary {
private:
    HMODULE handle_;
    std::string filePath_;

public:
    explicit DynamicLibrary(const std::string& filePath);
    ~DynamicLibrary();

    DynamicLibrary(const DynamicLibrary&) = delete;
    DynamicLibrary& operator=(const DynamicLibrary&) = delete;

    DynamicLibrary(DynamicLibrary&& other) noexcept;
    DynamicLibrary& operator=(DynamicLibrary&& other) noexcept;

    // Получение указателя на функцию из DLL
    void* getFunctionPointer(const std::string& functionName);

    // Безопасное получение функции с проверкой типа через шаблон
    template<typename FunctionType>
    FunctionType getFunction(const std::string& functionName) {
        void* funcPtr = getFunctionPointer(functionName);
        if (!funcPtr) {
            return nullptr;
        }
        
        // Используем union для преобразования типов
        union FunctionConverter {
            void* ptr;
            FunctionType func;
            
            FunctionConverter() : ptr(nullptr) {}
        } converter;
        
        converter.ptr = funcPtr;
        return converter.func;
    }

    bool isLoaded() const;
    const std::string& getFilePath() const;
};