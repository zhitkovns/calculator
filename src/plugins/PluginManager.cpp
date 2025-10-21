#include "PluginManager.h"
#include <filesystem>
#include <iostream>
#include <stdexcept>

// Тип функции для создания операции из DLL
using CreateOperationFunc = IOperation*(*)();

void PluginManager::loadPlugins(const std::string& pluginsDir) {
    // Очищаем предыдущие плагины
    clear();
    
    // Проверяем существование папки
    if (!std::filesystem::exists(pluginsDir)) {
        std::cout << "The plugins folder was not found: " << pluginsDir << std::endl;
        return;
    }
    
    if (!std::filesystem::is_directory(pluginsDir)) {
        std::cerr << "The specified path is not a folder: " << pluginsDir << std::endl;
        return;
    }
    
    // Проходим по всем файлам в папке
    for (const auto& entry : std::filesystem::directory_iterator(pluginsDir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".dll") {
            try {
                loadPlugin(entry.path().string());
                std::cout << "The plugin is loaded: " << entry.path().filename() << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Error loading the plugin " << entry.path().filename() 
                         << ": " << e.what() << std::endl;
            }
        }
    }
}

void PluginManager::loadPlugin(const std::string& filePath) {
    // Загружаем DLL
    auto library = std::make_unique<DynamicLibrary>(filePath);
    
    // Получаем функцию создания операции
    auto createFunc = library->getFunction<CreateOperationFunc>("createOperation");
    if (!createFunc) {
        throw std::runtime_error("The 'createOperation' function is not found in the DLL");
    }
    
    // Создаем операцию через функцию из DLL
    IOperation* rawOperation = createFunc();
    if (!rawOperation) {
        throw std::runtime_error("The DLL returned a nullptr for the operation");
    }
    
    std::unique_ptr<IOperation> operation(rawOperation);
    std::string operationName = operation->getName();
    
    // Проверяем уникальность имени операции
    if (operations_.find(operationName) != operations_.end()) {
        throw std::runtime_error("Operation with name '" + operationName + "' already registered");
    }
    
    // Регистрируем операцию и сохраняем библиотеку
    operations_[operationName] = std::move(operation);
    loadedLibraries_.push_back(std::move(library));
}

void PluginManager::registerOperation(std::unique_ptr<IOperation> operation) {
    if (!operation) {
        throw std::invalid_argument("Attempt to register a nullptr operation");
    }
    
    std::string name = operation->getName();
    if (operations_.find(name) != operations_.end()) {
        throw std::runtime_error("Operation with name '" + name + "' already registered");
    }
    
    operations_[name] = std::move(operation);
}

IOperation* PluginManager::getOperation(const std::string& name) const {
    auto it = operations_.find(name);
    if (it != operations_.end()) {
        return it->second.get();
    }
    return nullptr;
}

std::vector<std::string> PluginManager::getAvailableOperations() const {
    std::vector<std::string> result;
    for (const auto& pair : operations_) {
        result.push_back(pair.first);
    }
    return result;
}

bool PluginManager::hasOperation(const std::string& name) const {
    return operations_.find(name) != operations_.end();
}

void PluginManager::clear() {
    operations_.clear();
    loadedLibraries_.clear();
}