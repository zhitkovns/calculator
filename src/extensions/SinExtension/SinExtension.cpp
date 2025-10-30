#include "../../plugins/ExtensionAPI.h"
#include <cstdio>
#include <cmath>
#include <string>

static ExtensionMeta sin_extension_info;
static const HostServices* sin_host_services_ = nullptr;
static bool sin_extension_active = false;

static constexpr double MATH_PI = 3.14159265358979323846;

extern "C" __declspec(dllexport) int get_extension_metadata(ExtensionMeta** output_info) {
    
    // Дополнительные имена для функции sin
    static const char* extra_names[] = {"sin", "sin_func"};
    static size_t extra_name_sizes[] = {4, 9};
    
    // Настраиваем метаданные расширения
    sin_extension_info.abi_version = EXTENSION_ABI_VERSION;
    sin_extension_info.operation_name = "sin";
    sin_extension_info.name_length = 3;
    sin_extension_info.min_parameters = 1;
    sin_extension_info.max_parameters = 1;
    sin_extension_info.priority_level = 4; // Высший приоритет для функций
    sin_extension_info.is_operation = false;
    sin_extension_info.right_to_left = false;
    sin_extension_info.additional_names = extra_names;
    sin_extension_info.additional_name_lengths = extra_name_sizes;
    sin_extension_info.additional_name_count = 2;
    
    // Устанавливаем функцию вычисления
    sin_extension_info.compute = [](size_t param_count, const double* parameters, int* error_code, 
                                   char* error_text, size_t error_buffer_size) -> double
    {
        if (param_count != 1) {
            *error_code = 1;
            if (error_text && error_buffer_size > 0) {
                snprintf(error_text, error_buffer_size,
                    "Sin function expects 1 argument, got %zu", param_count);
            }
            return 0.0;
        }
        
        double angle_degrees = parameters[0];
        
        // Преобразуем градусы в радианы и вычисляем sin
        double angle_radians = (angle_degrees * MATH_PI) / 180.0;
        double result = std::sin(angle_radians);
        
        // Исправление ошибок округления для специальных углов
        const double epsilon = 1e-10;
        
        // Проверяем близость к нулю
        if (std::abs(result) < epsilon) {
            result = 0.0;
        }
        // Проверяем близость к 1
        else if (std::abs(result - 1.0) < epsilon) {
            result = 1.0;
        }
        // Проверяем близость к -1
        else if (std::abs(result + 1.0) < epsilon) {
            result = -1.0;
        }
        
        return result;
    };
    
    *output_info = &sin_extension_info;
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

    sin_host_services_ = services;
    sin_extension_active = true;

    if (sin_host_services_->write_log) {
        std::string init_msg = "Sin mathematical extension initialized successfully";
        sin_host_services_->write_log(init_msg.c_str(), init_msg.size());
    }

    return 0;
}

extern "C" __declspec(dllexport) void cleanup_extension() {
    if (sin_extension_active && sin_host_services_ && sin_host_services_->write_log) {
        std::string cleanup_msg = "Sin mathematical extension cleaned up";
        sin_host_services_->write_log(cleanup_msg.c_str(), cleanup_msg.size());
    }
    
    sin_extension_active = false;
    sin_host_services_ = nullptr;
}