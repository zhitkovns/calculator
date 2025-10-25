#pragma once
#include <cstddef>

// ABI version for compatibility
#define EXTENSION_ABI_VERSION 1

// Constants
constexpr size_t ERROR_MESSAGE_SIZE = 256;

// Host services provided to extensions
struct HostServices {
    void (*write_log)(const char* message, size_t message_size);
    void (*write_error)(const char* message, size_t message_size);
};

// Extension metadata
struct ExtensionMeta {
    int abi_version;                    // Must be EXTENSION_ABI_VERSION
    const char* operation_name;         // Operation name like "logarithm", "sine"
    size_t name_length;                 // Name length
    double (*compute)(size_t arg_count, const double* arguments, int* error_code, 
                     char* error_text, size_t error_buffer_size);
    int min_parameters;                 // Minimum parameters
    int max_parameters;                 // Maximum parameters (-1 = unlimited)
    unsigned priority_level;            // Priority 0-4
    bool is_operation;                  // Is this an operation like +, - etc.
    bool right_to_left;                 // Right-to-left associativity
    const char** additional_names;      // Additional names for this operation
    size_t* additional_name_lengths;    // Additional name lengths
    size_t additional_name_count;       // Number of additional names
};

// Extension function signatures
using get_extension_info_t = int(*)(ExtensionMeta** meta);
using extension_setup_t = int(*)(const HostServices* services, char* error_text, size_t error_buffer_size);
using extension_cleanup_t = void(*)();

// Operation categories
enum class OperationCategory {
    TWO_OPERAND,    // Binary operations
    SINGLE_OPERAND, // Unary operations  
    MATH_FUNCTION   // Functions
};

// Priority levels for operations
enum class PriorityLevel {
    BASIC = 0,      // +, -
    STANDARD = 1,   // 
    MEDIUM = 2,     // *, /
    HIGH = 3,       // ^
    TOP = 4         // functions
};