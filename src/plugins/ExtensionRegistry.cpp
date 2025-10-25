#include "ExtensionRegistry.h"
#include "HostIntegration.h"
#include <chrono>
#include <algorithm>
#include <iostream>

ExtensionRegistry::ExtensionRegistry() {
    setupHostServices();
}

ExtensionRegistry::~ExtensionRegistry() {
    for (auto& entry : extensions_by_path_) {
        auto extension = entry.second;
        if (extension) {
            extension->deactivate();
        }
    }
    extensions_by_name_.clear();
    extensions_by_path_.clear();
    removeBackupFiles();
}

bool ExtensionRegistry::registerExtension(const std::string& library_path, std::string* error_output) {
    // Create extension unit
    auto extension = std::make_shared<ExtensionUnit>();
    
    // Load the extension
    if (!extension->loadExtension(library_path, error_output)) {
        return false;
    }
    
    // Check for name conflicts
    for (const auto& name : extension->getAllNames()) {
        std::string standardized_name = standardizeName(name);
        if (extensions_by_name_.count(standardized_name)) {
            if (error_output) *error_output = "Name conflict: " + standardized_name;
            return false;
        }
    }
    
    // Activate the extension
    if (!extension->activate(&host_services_, error_output)) {
        return false;
    }
    
    // Register in maps
    for (const auto& name : extension->getAllNames()) {
        std::string standardized_name = standardizeName(name);
        extensions_by_name_[standardized_name] = extension;
    }
    extensions_by_path_[library_path] = extension;
    
    return true;
}

bool ExtensionRegistry::removeExtension(const std::string& operation_name, std::string* error_output) {
    std::string standardized_name = standardizeName(operation_name);
    auto it = extensions_by_name_.find(standardized_name);
    if (it == extensions_by_name_.end()) {
        if (error_output) *error_output = "Extension not found: " + operation_name;
        return false;
    }
    
    // Get the extension to remove it from path map as well
    auto extension = it->second;
    std::string library_path = extension->getLibraryPath();
    
    // Remove from name map
    extensions_by_name_.erase(it);
    
    // Remove from path map if no other names reference it
    bool found_in_names = false;
    for (const auto& entry : extensions_by_name_) {
        if (entry.second->getLibraryPath() == library_path) {
            found_in_names = true;
            break;
        }
    }
    
    if (!found_in_names) {
        extensions_by_path_.erase(library_path);
    }
    
    extension->deactivate();
    return true;
}

bool ExtensionRegistry::removeExtensionByPath(const std::string& library_path, std::string* error_output) {
    auto it = extensions_by_path_.find(library_path);
    if (it == extensions_by_path_.end()) {
        if (error_output) *error_output = "Extension path not found: " + library_path;
        return false;
    }
    
    auto extension = it->second;
    
    // Remove all names associated with this extension
    for (const auto& name : extension->getAllNames()) {
        std::string standardized_name = standardizeName(name);
        extensions_by_name_.erase(standardized_name);
    }
    
    // Remove from path map
    extensions_by_path_.erase(it);
    
    extension->deactivate();
    return true;
}

void ExtensionRegistry::scanExtensionsDirectory(const std::string& directory) {
    std::error_code ec;
    
    // Check if directory exists
    if (!std::filesystem::exists(directory)) {
        return;
    }
    
    for (const auto& entry : std::filesystem::directory_iterator(directory, ec)) {
        if (ec) {
            continue;
        }
        
        if (!entry.is_regular_file(ec) || ec) {
            continue;
        }
        
        auto file_path = entry.path();
        
        // Check if it's a valid library file
        if (!isValidLibraryFile(file_path)) {
            continue;
        }
        
        std::string path_string = file_path.string();
        
        // Check if already loaded
        if (extensions_by_path_.count(path_string)) {
            continue;
        }
        
        // Try to load the extension
        std::string error_msg;
        if (registerExtension(path_string, &error_msg)) {
            // Successfully loaded
        }
    }
}

std::shared_ptr<ExtensionUnit> ExtensionRegistry::findExtension(const std::string& operation_name) const {
    std::string standardized_name = standardizeName(operation_name);
    auto it = extensions_by_name_.find(standardized_name);
    if (it != extensions_by_name_.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<std::string> ExtensionRegistry::getAvailableExtensions() const {
    std::vector<std::string> extensions;
    for (const auto& entry : extensions_by_name_) {
        extensions.push_back(entry.first);
    }
    return extensions;
}

bool ExtensionRegistry::extensionExists(const std::string& operation_name) const {
    return extensions_by_name_.count(standardizeName(operation_name)) > 0;
}

double ExtensionRegistry::executeExtension(const std::string& operation_name, 
                                         const std::vector<double>& parameters, 
                                         std::string* error_output) {
    auto extension = findExtension(operation_name);
    if (!extension) {
        if (error_output) *error_output = "Extension not found: " + operation_name;
        return 0.0;
    }
    
    return extension->compute(parameters, error_output);
}

void ExtensionRegistry::refreshExtensions() {
    // Check for updated extensions
    for (auto& entry : extensions_by_path_) {
        const std::string& library_path = entry.first;
        auto extension = entry.second;
        
        std::error_code ec;
        auto current_mod_time = std::filesystem::last_write_time(library_path, ec);
        if (ec) {
            continue;
        }
        
        // Compare file_time_type directly - no conversion needed!
        if (current_mod_time > extension->getModTime()) {
            // File changed, reload it
            std::string error_msg;
            removeExtensionByPath(library_path, &error_msg);
            registerExtension(library_path, &error_msg);
        }
    }
}
std::string ExtensionRegistry::standardizeName(const std::string& name) const {
    std::string result = name;
    
    // Trim whitespace
    result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](unsigned char c) {
        return !std::isspace(c);
    }));
    result.erase(std::find_if(result.rbegin(), result.rend(), [](unsigned char c) {
        return !std::isspace(c);
    }).base(), result.end());
    
    // Convert to lowercase
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    
    return result;
}

bool ExtensionRegistry::isValidLibraryFile(const std::filesystem::path& file_path) const {
    auto filename = file_path.filename().string();
    
    // Skip hidden files, backups, temp files
    if (filename.empty() || filename[0] == '.' || 
        filename.find('~') != std::string::npos ||
        filename.find(".tmp") != std::string::npos ||
        filename.find(".bak") != std::string::npos) {
        return false;
    }
    
    // Check extension
    auto extension = file_path.extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    
    return extension == ".dll";
}

void ExtensionRegistry::removeBackupFiles() {
    for (auto& backup : backup_files_) {
        std::error_code ec;
        std::filesystem::remove(backup.second, ec);
    }
    backup_files_.clear();
}

void ExtensionRegistry::setupHostServices() {
    host_services_.write_log = &ExtensionRegistry::hostLogMessage;
    host_services_.write_error = &ExtensionRegistry::hostErrorReport;
}

void ExtensionRegistry::hostLogMessage(const char* message, size_t message_size) {
    if (message_size > 0) {
        // Could integrate with logger later
        std::cout << "[EXTENSION] " << std::string(message, message_size) << std::endl;
    }
}

void ExtensionRegistry::hostErrorReport(const char* message, size_t message_size) {
    if (message_size > 0) {
        std::cerr << "[EXTENSION ERROR] " << std::string(message, message_size) << std::endl;
    }
}