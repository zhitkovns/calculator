#pragma once
#include "ExtensionUnit.h"
#include "ExtensionAPI.h"
#include <unordered_map>
#include <filesystem>
#include <memory>
#include <string>

// Управляет всеми математическими расширениями
class ExtensionRegistry {
private:
    std::unordered_map<std::string, std::shared_ptr<ExtensionUnit>> extensions_by_name_;
    std::unordered_map<std::string, std::shared_ptr<ExtensionUnit>> extensions_by_path_;
    std::unordered_map<std::string, std::filesystem::path> backup_files_;
    HostServices host_services_;
    
public:
    ExtensionRegistry();
    ~ExtensionRegistry();
    
    // Запрет копирования для исключения проблем с владением плагинов
    ExtensionRegistry(const ExtensionRegistry&) = delete;
    ExtensionRegistry& operator=(const ExtensionRegistry&) = delete;

    // Управление расширениями
    bool registerExtension(const std::string& library_path, std::string* error_output = nullptr);
    bool removeExtension(const std::string& operation_name, std::string* error_output = nullptr);
    bool removeExtensionByPath(const std::string& library_path, std::string* error_output = nullptr);
    
    // Поиск и сканирование
    void scanExtensionsDirectory(const std::string& directory = "./extensions/");
    std::shared_ptr<ExtensionUnit> findExtension(const std::string& operation_name) const;
    std::vector<std::string> getAvailableExtensions() const;
    bool extensionExists(const std::string& operation_name) const;
    
    // Выполнение
    double executeExtension(const std::string& operation_name, 
                          const std::vector<double>& parameters, 
                          std::string* error_output = nullptr);
    
    // Динамические обновления
    void refreshExtensions();

private:
    // Вспомогательные методы
    std::string standardizeName(const std::string& name) const;
    bool isValidLibraryFile(const std::filesystem::path& file_path) const;
    void removeBackupFiles();
    void setupHostServices();
    
};