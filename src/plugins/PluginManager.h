#pragma once
#include "IOperation.h"
#include "DynamicLibrary.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

// Менеджер загрузки и управления плагинами
class PluginManager {
private:
    std::vector<std::unique_ptr<DynamicLibrary>> loadedLibraries_;
    std::unordered_map<std::string, std::unique_ptr<IOperation>> operations_;

public:
    PluginManager() = default;
    ~PluginManager() = default;

    // Запрещаем копирование
    PluginManager(const PluginManager&) = delete;
    PluginManager& operator=(const PluginManager&) = delete;

    // Загрузка всех плагинов из папки
    void loadPlugins(const std::string& pluginsDir = "./plugins/");

    // Регистрация операции
    void registerOperation(std::unique_ptr<IOperation> operation);

    // Получение операции по имени
    IOperation* getOperation(const std::string& name) const;

    // Получение списка всех доступных операций
    std::vector<std::string> getAvailableOperations() const;

    // Проверка существования операции
    bool hasOperation(const std::string& name) const;

    // Очистка всех плагинов
    void clear();

private:
    void loadPlugin(const std::string& filePath);
};