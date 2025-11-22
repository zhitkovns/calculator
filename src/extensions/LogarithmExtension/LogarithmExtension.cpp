#include "../../plugins/ExtensionAPI.h"
#include <cstdio>
#include <cmath>
#include <string>

static ExtensionMeta logarithm_extension_info;
static const HostServices* logarithm_host_services_ = nullptr;
static bool logarithm_extension_active = false;

extern "C" __declspec(dllexport) int get_extension_metadata(ExtensionMeta** output_info) {
    
    // Дополнительные имена для натурального логарифма
    static char const* extra_names[] = {"natural_log", "logarithm_natural"};
    static size_t extra_name_sizes[] = {11, 16};
    
    // Настраиваем метаданные расширения
    logarithm_extension_info.abi_version = EXTENSION_ABI_VERSION;
    logarithm_extension_info.operation_name = "ln";
    logarithm_extension_info.name_length = 2;
    logarithm_extension_info.min_parameters = 1;
    logarithm_extension_info.max_parameters = 1;
    logarithm_extension_info.priority_level = 4; // Высший приоритет для функций
    logarithm_extension_info.is_operation = false;
    logarithm_extension_info.right_to_left = false;
    logarithm_extension_info.additional_names = extra_names;
    logarithm_extension_info.additional_name_lengths = extra_name_sizes;
    logarithm_extension_info.additional_name_count = 2;
    
    // Устанавливаем функцию вычисления с обработкой исключений
    logarithm_extension_info.compute = [](size_t param_count, const double* parameters, int* error_code, 
                                        char* error_text, size_t error_buffer_size) -> double
    {
        // Сбрасываем код ошибки и сообщение
        *error_code = 0;
        if (error_text && error_buffer_size > 0) {
            error_text[0] = '\0';
        }
        
        // Проверяем количество параметров
        if (param_count != 1) {
            *error_code = 1;
            if (error_text && error_buffer_size > 0) {
                snprintf(error_text, error_buffer_size,
                    "Natural logarithm requires exactly 1 parameter, received %zu", param_count);
            }
            return 0.0;
        }
        
        double input_value = parameters[0];
        
        // Проверяем область определения
        if (input_value <= 0.0) {
            *error_code = 2;
            if (error_text && error_buffer_size > 0) {
                if (input_value == 0.0) {
                    snprintf(error_text, error_buffer_size,
                        "Natural logarithm undefined for zero value");
                } else if (input_value < 0.0) {
                    snprintf(error_text, error_buffer_size,
                        "Natural logarithm undefined for negative values: %.6f", input_value);
                } else {
                    snprintf(error_text, error_buffer_size,
                        "Natural logarithm domain error for input: %.6f", input_value);
                }
            }
            return 0.0;
        }
        
        double result = 0.0;
        
        // Вычисляем с безопасностью исключений
        try {
            result = std::log(input_value);
            
            // Проверяем на математические ошибки
            if (std::isnan(result) || std::isinf(result)) {
                *error_code = 3;
                if (error_text && error_buffer_size > 0) {
                    snprintf(error_text, error_buffer_size,
                        "Mathematical error in logarithm computation for input: %.6f", input_value);
                }
                return 0.0;
            }
            
        } catch (const std::exception& e) {
            *error_code = 4;
            if (error_text && error_buffer_size > 0) {
                snprintf(error_text, error_buffer_size,
                    "Exception in logarithm computation: %s", e.what());
            }
            return 0.0;
        } catch (...) {
            *error_code = 5;
            if (error_text && error_buffer_size > 0) {
                snprintf(error_text, error_buffer_size,
                    "Unknown exception in logarithm computation");
            }
            return 0.0;
        }
        
        return result;
    };
    
    *output_info = &logarithm_extension_info;
    return 0;
}

extern "C" __declspec(dllexport) int initialize_extension(const HostServices* services, 
                                                         char* error_text,
                                                         size_t error_buffer_size) {
    // Сбрасываем сообщение об ошибке
    if (error_text && error_buffer_size > 0) {
        error_text[0] = '\0';
    }
    
    if (!services) {
        if (error_text && error_buffer_size > 0) {
            snprintf(error_text, error_buffer_size, "Host services interface not provided");
        }
        return 1;
    }

    if (!services->write_log || !services->write_error) {
        if (error_text && error_buffer_size > 0) {
            snprintf(error_text, error_buffer_size, "Invalid host services interface");
        }
        return 1;
    }

    try {
        logarithm_host_services_ = services;
        logarithm_extension_active = true;

        if (logarithm_host_services_->write_log) {
            std::string init_msg = "Natural logarithm extension initialized successfully";
            logarithm_host_services_->write_log(init_msg.c_str(), init_msg.size());
        }

        return 0;
        
    } catch (const std::exception& e) {
        if (error_text && error_buffer_size > 0) {
            snprintf(error_text, error_buffer_size,
                "Exception during logarithm extension initialization: %s", e.what());
        }
        return 1;
    } catch (...) {
        if (error_text && error_buffer_size > 0) {
            snprintf(error_text, error_buffer_size,
                "Unknown exception during logarithm extension initialization");
        }
        return 1;
    }
}

extern "C" __declspec(dllexport) void cleanup_extension() {
    try {
        if (logarithm_extension_active && logarithm_host_services_ && logarithm_host_services_->write_log) {
            std::string cleanup_msg = "Natural logarithm extension cleanup completed";
            logarithm_host_services_->write_log(cleanup_msg.c_str(), cleanup_msg.size());
        }
    } catch (...) {
        // Подавляем все исключения во время очистки
    }
    
    logarithm_extension_active = false;
    logarithm_host_services_ = nullptr;
}