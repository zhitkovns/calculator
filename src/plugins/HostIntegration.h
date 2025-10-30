#pragma once
#include "ExtensionAPI.h"
#include <string>

namespace HostIntegration {
    // Настройка сервисов хоста для расширений
    void configureHostServices(HostServices* services);
    
    // Утилиты валидации
    bool validateExtensionMeta(const ExtensionMeta* meta, std::string* error_output = nullptr);
    std::string getSystemError();
}