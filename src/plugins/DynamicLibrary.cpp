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

// Move constructor
DynamicLibrary::DynamicLibrary(DynamicLibrary&& other) noexcept 
    : handle_(other.handle_), filePath_(std::move(other.filePath_)) {
    other.handle_ = nullptr;
}

// Move assignment
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