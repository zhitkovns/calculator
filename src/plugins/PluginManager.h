#pragma once
#include "ExtensionRegistry.h"
#include <string>

class PluginManager {
private:
    ExtensionRegistry extensionRegistry_;

public:
    PluginManager() = default;
    
    void loadPlugins() {
        extensionRegistry_.scanExtensionsDirectory("./extensions/");
    }
    
    ExtensionRegistry& getRegistry() { return extensionRegistry_; }
    const ExtensionRegistry& getRegistry() const { return extensionRegistry_; }
};