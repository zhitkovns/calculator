#include "ExtensionUnit.h"
#include "HostIntegration.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>

ExtensionUnit::~ExtensionUnit() {
    releaseResources();
}

ExtensionUnit::ExtensionUnit(ExtensionUnit&& other) noexcept
    : library_unit_(std::move(other.library_unit_))
    , library_path_(std::move(other.library_path_))
    , operation_names_(std::move(other.operation_names_))
    , meta_data_(other.meta_data_)
    , get_info_func_(other.get_info_func_)
    , setup_func_(other.setup_func_)
    , cleanup_func_(other.cleanup_func_)
    , is_active_(other.is_active_)
    , modification_time_(other.modification_time_) {  // Копируем напрямую
    other.meta_data_ = nullptr;
    other.get_info_func_ = nullptr;
    other.setup_func_ = nullptr;
    other.cleanup_func_ = nullptr;
    other.is_active_ = false;
}

ExtensionUnit& ExtensionUnit::operator=(ExtensionUnit&& other) noexcept {
    if (this != &other) {
        releaseResources();
        library_unit_ = std::move(other.library_unit_);
        library_path_ = std::move(other.library_path_);
        operation_names_ = std::move(other.operation_names_);
        meta_data_ = other.meta_data_;
        get_info_func_ = other.get_info_func_;
        setup_func_ = other.setup_func_;
        cleanup_func_ = other.cleanup_func_;
        is_active_ = other.is_active_;
        modification_time_ = other.modification_time_;  // Копируем напрямую
        
        other.meta_data_ = nullptr;
        other.get_info_func_ = nullptr;
        other.setup_func_ = nullptr;
        other.cleanup_func_ = nullptr;
        other.is_active_ = false;
    }
    return *this;
}

bool ExtensionUnit::loadExtension(const std::string& library_path, std::string* error_output) {
    releaseResources();
    
    try {
        // Load the dynamic library
        library_unit_ = std::make_shared<DynamicLibrary>(library_path);
        library_path_ = library_path;
        
        // Find the metadata function
        get_info_func_ = library_unit_->getFunction<get_extension_info_t>("get_extension_metadata");
        if (!get_info_func_) {
            if (error_output) *error_output = "get_extension_metadata not found in " + library_path;
            releaseResources();
            return false;
        }
        
        // Retrieve extension metadata
        ExtensionMeta* meta = nullptr;
        int status = get_info_func_(&meta);
        if (status != 0 || !meta) {
            if (error_output) *error_output = "get_extension_metadata failed for " + library_path;
            releaseResources();
            return false;
        }
        
        // Validate metadata
        if (!HostIntegration::validateExtensionMeta(meta, error_output)) {
            releaseResources();
            return false;
        }
        
        meta_data_ = meta;
        
        // Find lifecycle functions
        setup_func_ = library_unit_->getFunction<extension_setup_t>("initialize_extension");
        cleanup_func_ = library_unit_->getFunction<extension_cleanup_t>("cleanup_extension");
        
        // Collect all operation names
        operation_names_.clear();
        if (meta_data_->operation_name && meta_data_->name_length > 0) {
            operation_names_.emplace_back(meta_data_->operation_name, meta_data_->name_length);
        }
        
        // Add additional names
        if (meta_data_->additional_name_count > 0 && meta_data_->additional_names && meta_data_->additional_name_lengths) {
            for (size_t i = 0; i < meta_data_->additional_name_count; ++i) {
                if (meta_data_->additional_names[i] && meta_data_->additional_name_lengths[i] > 0) {
                    operation_names_.emplace_back(meta_data_->additional_names[i], meta_data_->additional_name_lengths[i]);
                }
            }
        }
        
        // Record file modification time
        std::error_code ec;
        modification_time_ = std::filesystem::last_write_time(library_path, ec);
        if (ec) {
            modification_time_ = std::filesystem::file_time_type::min();
        }
        
        return true;
        
    } catch (const std::exception& e) {
        if (error_output) *error_output = std::string("Failed to load extension: ") + e.what();
        releaseResources();
        return false;
    }
}

void ExtensionUnit::unloadExtension() {
    deactivate();
    releaseResources();
}

bool ExtensionUnit::activate(const HostServices* services, std::string* error_output) {
    if (!isLoaded() || is_active_) {
        return false;
    }
    
    if (setup_func_) {
        char error_buffer[ERROR_MESSAGE_SIZE] = {0};
        int result = setup_func_(services, error_buffer, ERROR_MESSAGE_SIZE);
        if (result != 0) {
            if (error_output) {
                *error_output = std::string(error_buffer[0] ? error_buffer : "initialize_extension failed");
            }
            return false;
        }
    }
    
    is_active_ = true;
    return true;
}

void ExtensionUnit::deactivate() {
    if (is_active_ && cleanup_func_) {
        try {
            cleanup_func_();
        } catch (...) {
            // Suppress exceptions during cleanup
        }
        is_active_ = false;
    }
}

std::string ExtensionUnit::getPrimaryName() const {
    if (meta_data_ && meta_data_->operation_name && meta_data_->name_length > 0) {
        return std::string(meta_data_->operation_name, meta_data_->name_length);
    }
    return "";
}

std::vector<std::string> ExtensionUnit::getAllNames() const {
    return operation_names_;
}

std::pair<int, int> ExtensionUnit::getParameterRange() const {
    if (meta_data_) {
        return {meta_data_->min_parameters, meta_data_->max_parameters};
    }
    return {0, 0};
}

PriorityLevel ExtensionUnit::getPriority() const {
    if (meta_data_ && meta_data_->is_operation) {
        return static_cast<PriorityLevel>(std::min(meta_data_->priority_level, 4u));
    }
    return PriorityLevel::TOP;
}

bool ExtensionUnit::isMathOperator() const {
    return meta_data_ && meta_data_->is_operation;
}

bool ExtensionUnit::hasRightAssociativity() const {
    return meta_data_ && meta_data_->right_to_left;
}

OperationCategory ExtensionUnit::getCategory() const {
    if (!meta_data_) {
        return OperationCategory::MATH_FUNCTION;
    }
    
    if (meta_data_->is_operation) {
        if (meta_data_->min_parameters == 1 && meta_data_->max_parameters == 1) {
            return OperationCategory::SINGLE_OPERAND;
        } else if (meta_data_->min_parameters == 2 && meta_data_->max_parameters == 2) {
            return OperationCategory::TWO_OPERAND;
        }
        return OperationCategory::TWO_OPERAND;
    }
    
    return OperationCategory::MATH_FUNCTION;
}

double ExtensionUnit::compute(const std::vector<double>& parameters, std::string* error_output) {
    if (!meta_data_ || !meta_data_->compute) {
        if (error_output) *error_output = "Extension not properly loaded";
        return 0.0;
    }
    
    // Validate parameter count
    size_t param_count = parameters.size();
    if (!((meta_data_->max_parameters == -1 && param_count >= static_cast<size_t>(meta_data_->min_parameters)) ||
          (param_count >= static_cast<size_t>(meta_data_->min_parameters) && 
           param_count <= static_cast<size_t>(meta_data_->max_parameters)))) {
        if (error_output) *error_output = "Parameter count does not match requirements";
        return 0.0;
    }
    
    try {
        int error_code = 0;
        char error_text[ERROR_MESSAGE_SIZE] = {0};
        
        double result = meta_data_->compute(param_count, parameters.data(), &error_code, error_text, ERROR_MESSAGE_SIZE);
        
        if (error_code != 0) {
            if (error_output) {
                *error_output = error_text[0] ? std::string(error_text) : "Extension computation failed";
            }
            return 0.0;
        }
        
        return result;
        
    } catch (const std::exception& e) {
        if (error_output) *error_output = std::string("Extension exception: ") + e.what();
        return 0.0;
    } catch (...) {
        if (error_output) *error_output = "Unknown extension exception";
        return 0.0;
    }
}

void ExtensionUnit::releaseResources() {
    deactivate();
    library_unit_.reset();
    meta_data_ = nullptr;
    get_info_func_ = nullptr;
    setup_func_ = nullptr;
    cleanup_func_ = nullptr;
    operation_names_.clear();
    library_path_.clear();
    is_active_ = false;
}

bool ExtensionUnit::isRightAssociative() const {
    return meta_data_ && meta_data_->right_to_left;
}