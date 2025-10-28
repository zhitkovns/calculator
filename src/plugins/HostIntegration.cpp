#include "HostIntegration.h"
#include <iostream>
#include <windows.h>


namespace HostIntegration {

void configureHostServices(HostServices* services) {
    services->write_log = [](const char* message, size_t message_size) {
        if (message_size > 0) {
            std::cout << "[HOST] " << std::string(message, message_size) << std::endl;
        }
    };
    
    services->write_error = [](const char* message, size_t message_size) {
        if (message_size > 0) {
            std::cerr << "[HOST ERROR] " << std::string(message, message_size) << std::endl;
        }
    };
}

bool validateExtensionMeta(const ExtensionMeta* meta, std::string* error_output) {
    if (!meta) {
        if (error_output) *error_output = "Null metadata";
        return false;
    }
    
    // Check ABI version
    if (meta->abi_version != EXTENSION_ABI_VERSION) {
        if (error_output) *error_output = "ABI version mismatch";
        return false;
    }
    
    // Check name
    if (!meta->operation_name || meta->name_length == 0) {
        if (error_output) *error_output = "Invalid operation name";
        return false;
    }
    
    // Check function pointer
    if (!meta->compute) {
        if (error_output) *error_output = "No compute function provided";
        return false;
    }
    
    // Validate parameter range
    if (meta->min_parameters < 0 || 
        (meta->max_parameters != -1 && meta->max_parameters < meta->min_parameters)) {
        if (error_output) *error_output = "Invalid parameter range";
        return false;
    }
    
    // Validate alternative names
    if (meta->additional_name_count > 0) {
        if (!meta->additional_names || !meta->additional_name_lengths) {
            if (error_output) *error_output = "Invalid alternative names structure";
            return false;
        }
        
        for (size_t i = 0; i < meta->additional_name_count; ++i) {
            if (!meta->additional_names[i] || meta->additional_name_lengths[i] == 0) {
                if (error_output) *error_output = "Invalid alternative name";
                return false;
            }
        }
    }
    
    return true;
}

std::string getSystemError() {
    DWORD error_code = GetLastError();
    if (error_code == 0) return {};
    
    LPSTR message_buffer = nullptr;
    DWORD length = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&message_buffer, 0, nullptr);
    
    std::string result;
    if (length && message_buffer) {
        result.assign(message_buffer, message_buffer + length);
        LocalFree(message_buffer);
    }
    
    return result;
}

} // namespace HostIntegration