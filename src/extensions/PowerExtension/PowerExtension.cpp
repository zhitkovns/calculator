#include "../../plugins/ExtensionAPI.h"
#include <cstdio>
#include <cmath>
#include <string>

static ExtensionMeta power_info;
static const HostServices* power_host_ = nullptr;
static bool power_active = false;

extern "C" __declspec(dllexport) int get_extension_metadata(ExtensionMeta** output_info) {
    
    // Дополнительные имена для оператора степени
    static const char* extra_names[] = {"pow", "exponent"};
    static size_t extra_name_sizes[] = {3, 8};
    
    // Настраиваем метаданные расширения
    power_info.abi_version = EXTENSION_ABI_VERSION;
    power_info.operation_name = "^";
    power_info.name_length = 1;
    power_info.min_parameters = 2;
    power_info.max_parameters = 2;
    power_info.priority_level = 3;
    power_info.is_operation = true;
    power_info.right_to_left = false;
    power_info.additional_names = extra_names;
    power_info.additional_name_lengths = extra_name_sizes;
    power_info.additional_name_count = 2;
    
    // Устанавливаем функцию вычисления с обработкой ошибок
    power_info.compute = [](size_t param_count, const double* parameters, int* error_code, 
                          char* error_text, size_t error_buffer_size) -> double
    {
        if (param_count != 2) {
            *error_code = 1;
            if (error_text && error_buffer_size > 0) {
                snprintf(error_text, error_buffer_size,
                    "Power operator requires 2 arguments, got %zu", param_count);
            }
            return 0.0;
        }
        
        double base = parameters[0];
        double exponent = parameters[1];
        double result = 0.0;
        
        // Обрабатываем специальные случаи и ошибки
        try {
            // Проверяем математические ошибки области определения
            if (base == 0.0 && exponent < 0.0) {
                *error_code = 2;
                if (error_text && error_buffer_size > 0) {
                    snprintf(error_text, error_buffer_size,
                        "Division by zero: cannot raise 0 to negative power");
                }
                return 0.0;
            }
            
            if (base < 0.0 && std::fmod(exponent, 1.0) != 0.0) {
                *error_code = 3;
                if (error_text && error_buffer_size > 0) {
                    snprintf(error_text, error_buffer_size,
                        "Negative base with fractional exponent is undefined");
                }
                return 0.0;
            }
            
            result = std::pow(base, exponent);
            
            // Проверяем переполнение/потерю точности
            if (std::isinf(result)) {
                *error_code = 4;
                if (error_text && error_buffer_size > 0) {
                    snprintf(error_text, error_buffer_size,
                        "Result overflow in power operation");
                }
                return 0.0;
            }
            
            if (std::isnan(result)) {
                *error_code = 5;
                if (error_text && error_buffer_size > 0) {
                    snprintf(error_text, error_buffer_size,
                        "Mathematical error in power operation");
                }
                return 0.0;
            }
            
        } catch (const std::exception& e) {
            *error_code = 6;
            if (error_text && error_buffer_size > 0) {
                snprintf(error_text, error_buffer_size,
                    "Exception in power computation: %s", e.what());
            }
            return 0.0;
        } catch (...) {
            *error_code = 7;
            if (error_text && error_buffer_size > 0) {
                snprintf(error_text, error_buffer_size,
                    "Unknown exception in power computation");
            }
            return 0.0;
        }
        
        return result;
    };
    
    *output_info = &power_info;
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

    power_host_ = services;
    power_active = true;

    if (power_host_->write_log) {
        std::string init_msg = "Power operator extension initialized successfully";
        power_host_->write_log(init_msg.c_str(), init_msg.size());
    }

    return 0;
}

extern "C" __declspec(dllexport) void cleanup_extension() {
    if (power_active && power_host_ && power_host_->write_log) {
        std::string cleanup_msg = "Power operator extension cleaned up";
        power_host_->write_log(cleanup_msg.c_str(), cleanup_msg.size());
    }
    
    power_active = false;
    power_host_ = nullptr;
}