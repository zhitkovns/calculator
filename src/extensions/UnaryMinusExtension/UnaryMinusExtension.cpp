#include "../../plugins/ExtensionAPI.h"
#include <cstdio>
#include <cmath>
#include <string>

static ExtensionMeta unary_minus_info;
static HostServices* unary_minus_host_ = nullptr;
static bool unary_minus_active = false;

extern "C" __declspec(dllexport) int get_extension_metadata(ExtensionMeta** output_info) {
    
    // Additional names for unary minus
    static const char* extra_names[] = {"negate", "negative"};
    static size_t extra_name_sizes[] = {6, 8};
    
    // Configure extension metadata
    unary_minus_info.abi_version = EXTENSION_ABI_VERSION;
    unary_minus_info.operation_name = "-";
    unary_minus_info.name_length = 1;
    unary_minus_info.min_parameters = 1;
    unary_minus_info.max_parameters = 1;
    unary_minus_info.priority_level = 4; // High priority for unary operations
    unary_minus_info.is_operation = true;
    unary_minus_info.right_to_left = false;
    unary_minus_info.additional_names = extra_names;
    unary_minus_info.additional_name_lengths = extra_name_sizes;
    unary_minus_info.additional_name_count = 2;
    
    // Set the computation function
    unary_minus_info.compute = [](size_t param_count, const double* parameters, int* error_code, 
                                char* error_text, size_t error_buffer_size) -> double
    {
        if (param_count != 1) {
            *error_code = 1;
            if (error_text && error_buffer_size > 0) {
                snprintf(error_text, error_buffer_size,
                    "Unary minus requires 1 argument, got %zu", param_count);
            }
            return 0.0;
        }
        
        double result = -parameters[0];
        
        // Log the computation
        if (unary_minus_host_ && unary_minus_host_->write_log) {
            std::string log_msg = "Unary minus computed: -(" + 
                                 std::to_string(parameters[0]) + ") = " + 
                                 std::to_string(result);
            unary_minus_host_->write_log(log_msg.c_str(), log_msg.size());
        }
        
        return result;
    };
    
    *output_info = &unary_minus_info;
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

    unary_minus_host_ = const_cast<HostServices*>(services);
    unary_minus_active = true;

    if (unary_minus_host_->write_log) {
        std::string init_msg = "Unary minus extension initialized successfully";
        unary_minus_host_->write_log(init_msg.c_str(), init_msg.size());
    }

    return 0;
}

extern "C" __declspec(dllexport) void cleanup_extension() {
    if (unary_minus_active && unary_minus_host_ && unary_minus_host_->write_log) {
        std::string cleanup_msg = "Unary minus extension cleaned up";
        unary_minus_host_->write_log(cleanup_msg.c_str(), cleanup_msg.size());
    }
    
    unary_minus_active = false;
    unary_minus_host_ = nullptr;
}