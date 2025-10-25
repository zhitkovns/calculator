#pragma once
#include "ExtensionAPI.h"
#include <string>

namespace HostIntegration {
    // Setup host services for extensions
    void configureHostServices(HostServices* services);
    
    // Validation utilities
    bool validateExtensionMeta(const ExtensionMeta* meta, std::string* error_output = nullptr);
    std::string getSystemError();
}