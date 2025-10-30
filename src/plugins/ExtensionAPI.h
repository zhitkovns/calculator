#pragma once
#include <cstddef>

// Версия ABI для совместимости
#define EXTENSION_ABI_VERSION 1

// Константы
constexpr size_t ERROR_MESSAGE_SIZE = 256;

// Сервисы хоста, предоставляемые расширениям
struct HostServices {
    void (*write_log)(const char* message, size_t message_size);
    void (*write_error)(const char* message, size_t message_size);
};

// Метаданные расширения
struct ExtensionMeta {
    int abi_version;                    // Должна быть EXTENSION_ABI_VERSION
    const char* operation_name;         // Имя операции, например "logarithm", "sin"
    size_t name_length;                 // Длина имени
    double (*compute)(size_t arg_count, const double* arguments, int* error_code, 
                     char* error_text, size_t error_buffer_size);
    int min_parameters;                 // Минимальное количество параметров
    int max_parameters;                 // Максимальное количество параметров (-1 = неограничено)
    unsigned priority_level;            // Уровень приоритета 0-4
    bool is_operation;                  // Является ли операцией типа +, - и т.д.
    bool right_to_left;                 // Ассоциативность справа налево
    const char** additional_names;      // Дополнительные имена для этой операции
    size_t* additional_name_lengths;    // Длины дополнительных имен
    size_t additional_name_count;       // Количество дополнительных имен
};

// Сигнатуры функций расширения
using get_extension_info_t = int(*)(ExtensionMeta** meta);
using extension_setup_t = int(*)(const HostServices* services, char* error_text, size_t error_buffer_size);
using extension_cleanup_t = void(*)();

// Категории операций
enum class OperationCategory {
    TWO_OPERAND,    // Бинарные операции
    SINGLE_OPERAND, // Унарные операции  
    MATH_FUNCTION   // Функции
};

// Уровни приоритета для операций
enum class PriorityLevel {
    BASIC = 0,      // +, -
    STANDARD = 1,   // 
    MEDIUM = 2,     // *, /
    HIGH = 3,       // ^
    TOP = 4         // функции
};