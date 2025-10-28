#include "../../plugins/ExtensionAPI.h"
#include <cstdio>
#include <cmath>
#include <string>

static ExtensionMeta cos_extension_info;
static HostServices* cos_host_services_ = nullptr;
static bool cos_extension_active = false;

static constexpr double MATH_PI = 3.14159265358979323846;

extern "C" __declspec(dllexport) int get_extension_metadata(ExtensionMeta** output_info) {
    
    // Additional names for the cos function
    static const char* extra_names[] = {"cos", "cos_func"};
    static size_t extra_name_sizes[] = {4, 9};
    
    // Configure extension metadata
    cos_extension_info.abi_version = EXTENSION_ABI_VERSION;
    cos_extension_info.operation_name = "cos";
    cos_extension_info.name_length = 3;
    cos_extension_info.min_parameters = 1;
    cos_extension_info.max_parameters = 1;
    cos_extension_info.priority_level = 4; // Highest priority for functions
    cos_extension_info.is_operation = false;
    cos_extension_info.right_to_left = false;
    cos_extension_info.additional_names = extra_names;
    cos_extension_info.additional_name_lengths = extra_name_sizes;
    cos_extension_info.additional_name_count = 2;
    
    // Set the computation function
    cos_extension_info.compute = [](size_t param_count, const double* parameters, int* error_code, 
                                   char* error_text, size_t error_buffer_size) -> double
    {
        if (param_count != 1) {
            *error_code = 1;
            if (error_text && error_buffer_size > 0) {
                snprintf(error_text, error_buffer_size,
                    "cos function expects 1 argument, got %zu", param_count);
            }
            return 0.0;
        }
        
        double angle_degrees = parameters[0];
        
        // Convert degrees to radians and compute cos
        double angle_radians = (angle_degrees * MATH_PI) / 180.0;
        double result = std::cos(angle_radians);
        
        // Optional: log the computation
        if (cos_host_services_ && cos_host_services_->write_log) {
            std::string log_msg = "cos computed: cos(" + std::to_string(angle_degrees) + "°) = " + std::to_string(result);
            cos_host_services_->write_log(log_msg.c_str(), log_msg.size());
        }
        
        return result;
    };
    
    *output_info = &cos_extension_info;
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

    cos_host_services_ = const_cast<HostServices*>(services);
    cos_extension_active = true;

    return 0;
}

extern "C" __declspec(dllexport) void cleanup_extension() {
    if (cos_extension_active && cos_host_services_ && cos_host_services_->write_log) {
        std::string cleanup_msg = "Cos mathematical extension cleaned up";
        cos_host_services_->write_log(cleanup_msg.c_str(), cleanup_msg.size());
    }
    
    cos_extension_active = false;
    cos_host_services_ = nullptr;
}