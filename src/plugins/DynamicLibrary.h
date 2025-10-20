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

    // Запрещаем копирование
    DynamicLibrary(const DynamicLibrary&) = delete;
    DynamicLibrary& operator=(const DynamicLibrary&) = delete;

    // Разрешаем перемещение
    DynamicLibrary(DynamicLibrary&& other) noexcept;
    DynamicLibrary& operator=(DynamicLibrary&& other) noexcept;

    // Получение функции из DLL
    template<typename T>
    T getFunction(const std::string& functionName) {
        return reinterpret_cast<T>(GetProcAddress(handle_, functionName.c_str()));
    }

    bool isLoaded() const;
    const std::string& getFilePath() const;
};