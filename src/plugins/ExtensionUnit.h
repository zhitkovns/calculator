#pragma once
#include "ExtensionAPI.h"
#include "DynamicLibrary.h"
#include <string>
#include <vector>
#include <memory>
#include <filesystem>

// Представляет одно загруженное расширение
class ExtensionUnit {
private:
    std::shared_ptr<DynamicLibrary> library_unit_;
    std::string library_path_;
    std::vector<std::string> operation_names_;
    ExtensionMeta* meta_data_ = nullptr;
    get_extension_info_t get_info_func_ = nullptr;
    extension_setup_t setup_func_ = nullptr;
    extension_cleanup_t cleanup_func_ = nullptr;
    bool is_active_ = false;
    std::filesystem::file_time_type modification_time_;  // Используем родной тип

public:
    ExtensionUnit() = default;
    ~ExtensionUnit();
    
    // Запрещаем копирование
    ExtensionUnit(const ExtensionUnit&) = delete;
    ExtensionUnit& operator=(const ExtensionUnit&) = delete;
    
    // Разрешаем перемещение
    ExtensionUnit(ExtensionUnit&& other) noexcept;
    ExtensionUnit& operator=(ExtensionUnit&& other) noexcept;

    // Операции загрузки/выгрузки
    bool loadExtension(const std::string& library_path, std::string* error_output = nullptr);
    void unloadExtension();
    
    // Управление жизненным циклом
    bool activate(const HostServices* services, std::string* error_output = nullptr);
    void deactivate();
    
    // Информация о расширении
    std::string getPrimaryName() const;
    std::vector<std::string> getAllNames() const;
    std::pair<int, int> getParameterRange() const;
    PriorityLevel getPriority() const;
    bool isMathOperator() const;
    bool hasRightAssociativity() const;
    OperationCategory getCategory() const;
    
    // Выполнение функции расширения
    double compute(const std::vector<double>& parameters, std::string* error_output = nullptr);
    
    // Проверки статуса
    bool isLoaded() const { return library_unit_ != nullptr; }
    bool isActive() const { return is_active_; }
    const std::string& getLibraryPath() const { return library_path_; }
    std::filesystem::file_time_type getModTime() const { return modification_time_; }  // Возвращаем родной тип

    bool isRightAssociative() const;
    int getPriorityValue() const;

private:
    void releaseResources();
};