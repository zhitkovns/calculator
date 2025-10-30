#include "DynamicLibrary.h"
#include <stdexcept>

DynamicLibrary::DynamicLibrary(const std::string& filePath) : filePath_(filePath) {
    handle_ = LoadLibraryA(filePath.c_str());
    
    if (!handle_) {
        DWORD error = GetLastError();
        throw std::runtime_error("Failed to load DLL: " + filePath + " (Error: " + std::to_string(error) + ")");
    }
}

DynamicLibrary::~DynamicLibrary() {
    if (handle_) {
        FreeLibrary(handle_);
    }
}

// Конструктор перемещения
DynamicLibrary::DynamicLibrary(DynamicLibrary&& other) noexcept 
    : handle_(other.handle_), filePath_(std::move(other.filePath_)) {
    other.handle_ = nullptr;
}

// Оператор перемещения
DynamicLibrary& DynamicLibrary::operator=(DynamicLibrary&& other) noexcept {
    if (this != &other) {
        if (handle_) {
            FreeLibrary(handle_);
        }
        handle_ = other.handle_;
        filePath_ = std::move(other.filePath_);
        other.handle_ = nullptr;
    }
    return *this;
}

bool DynamicLibrary::isLoaded() const {
    return handle_ != nullptr;
}

const std::string& DynamicLibrary::getFilePath() const {
    return filePath_;
}

void* DynamicLibrary::getFunctionPointer(const std::string& functionName) {
    FARPROC proc = GetProcAddress(handle_, functionName.c_str());
    // Используем union для безопасного преобразования FARPROC в void*
    union {
        FARPROC farproc;
        void* ptr;
    } converter;
    converter.farproc = proc;
    return converter.ptr;
}