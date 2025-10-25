#pragma once
#include "ExtensionAPI.h"
#include "DynamicLibrary.h"
#include <string>
#include <vector>
#include <memory>

// Represents a single loaded extension
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
    std::filesystem::file_time_type modification_time_;

public:
    ExtensionUnit() = default;
    ~ExtensionUnit();
    
    // Prevent copying
    ExtensionUnit(const ExtensionUnit&) = delete;
    ExtensionUnit& operator=(const ExtensionUnit&) = delete;
    
    // Allow moving
    ExtensionUnit(ExtensionUnit&& other) noexcept;
    ExtensionUnit& operator=(ExtensionUnit&& other) noexcept;

    // Load/unload operations
    bool loadExtension(const std::string& library_path, std::string* error_output = nullptr);
    void unloadExtension();
    
    // Lifecycle management
    bool activate(const HostServices* services, std::string* error_output = nullptr);
    void deactivate();
    
    // Extension information
    std::string getPrimaryName() const;
    std::vector<std::string> getAllNames() const;
    std::pair<int, int> getParameterRange() const;
    PriorityLevel getPriority() const;
    bool isMathOperator() const;
    bool hasRightAssociativity() const;
    OperationCategory getCategory() const;
    
    // Execute the extension's function
    double compute(const std::vector<double>& parameters, std::string* error_output = nullptr);
    
    // Status checks
    bool isLoaded() const { return library_unit_ != nullptr; }
    bool isActive() const { return is_active_; }
    const std::string& getLibraryPath() const { return library_path_; }
    std::filesystem::file_time_type getModTime() const { return modification_time_; }

private:
    void releaseResources();
};