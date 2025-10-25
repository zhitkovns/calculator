#include "../../plugins/ExtensionAPI.h"
#include <cstdio>
#include <cmath>
#include <string>

static ExtensionMeta sine_extension_info;
static HostServices* sine_host_services_ = nullptr;
static bool sine_extension_active = false;

static constexpr double MATH_PI = 3.14159265358979323846;

extern "C" __declspec(dllexport) int get_extension_metadata(ExtensionMeta** output_info) {
    
    // Additional names for the sine function
    static const char* extra_names[] = {"sine", "sine_func"};
    static size_t extra_name_sizes[] = {4, 9};
    
    // Configure extension metadata
    sine_extension_info.abi_version = EXTENSION_ABI_VERSION;
    sine_extension_info.operation_name = "sin";
    sine_extension_info.name_length = 3;
    sine_extension_info.min_parameters = 1;
    sine_extension_info.max_parameters = 1;
    sine_extension_info.priority_level = 4; // Highest priority for functions
    sine_extension_info.is_operation = false;
    sine_extension_info.right_to_left = false;
    sine_extension_info.additional_names = extra_names;
    sine_extension_info.additional_name_lengths = extra_name_sizes;
    sine_extension_info.additional_name_count = 2;
    
    // Set the computation function
    sine_extension_info.compute = [](size_t param_count, const double* parameters, int* error_code, 
                                   char* error_text, size_t error_buffer_size) -> double
    {
        if (param_count != 1) {
            *error_code = 1;
            if (error_text && error_buffer_size > 0) {
                snprintf(error_text, error_buffer_size,
                    "Sine function expects 1 argument, got %zu", param_count);
            }
            return 0.0;
        }
        
        double angle_degrees = parameters[0];
        
        // Convert degrees to radians and compute sine
        double angle_radians = (angle_degrees * MATH_PI) / 180.0;
        double result = std::sin(angle_radians);
        
        // Optional: log the computation
        if (sine_host_services_ && sine_host_services_->write_log) {
            std::string log_msg = "Sine computed: sin(" + std::to_string(angle_degrees) + "°) = " + std::to_string(result);
            sine_host_services_->write_log(log_msg.c_str(), log_msg.size());
        }
        
        return result;
    };
    
    *output_info = &sine_extension_info;
    return 0;
}

extern "C" __declspec(dllexport) int initialize_extension(const HostServices* services, 
                                                         char* error_text,
                                                         size_t error_buffer_size) {
    if (!services) {
        if (error_text && error_buffer_size > 0) {
            snprintf(error_text, error_buffer_size, "Host services not provided");
        }
        return 1;
    }

    sine_host_services_ = const_cast<HostServices*>(services);
    sine_extension_active = true;

    if (sine_host_services_->write_log) {
        std::string init_msg = "Sine mathematical extension initialized successfully";
        sine_host_services_->write_log(init_msg.c_str(), init_msg.size());
    }

    return 0;
}

extern "C" __declspec(dllexport) void cleanup_extension() {
    if (sine_extension_active && sine_host_services_ && sine_host_services_->write_log) {
        std::string cleanup_msg = "Sine mathematical extension cleaned up";
        sine_host_services_->write_log(cleanup_msg.c_str(), cleanup_msg.size());
    }
    
    sine_extension_active = false;
    sine_host_services_ = nullptr;
}