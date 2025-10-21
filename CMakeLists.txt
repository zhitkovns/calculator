cmake_minimum_required(VERSION 3.15)
project(Calculator)

# Настройки компилятора
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включение всех предупреждений для MSVC
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

# Настройка выходной директории
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Создание папки plugins рядом с исполняемым файлом
add_custom_target(create_plugins_dir ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins
)

# Исходные файлы
set(SRC_FILES
    src/main.cpp
    src/core/Calculator.cpp
    src/core/ExpressionParser.cpp
    src/core/OperationFactory.cpp
    src/core/AST/NumberNode.cpp
    src/core/AST/BinaryNode.cpp
    src/core/AST/UnaryNode.cpp
    src/operations/BuiltinOperations.cpp
    src/plugins/PluginManager.cpp
    src/plugins/DynamicLibrary.cpp
)

# Заголовочные файлы
include_directories(
    src/
    src/core/
    src/core/AST/
    src/operations/
    src/plugins/
)

# Создание исполняемого файла
add_executable(calc ${SRC_FILES})

# Зависимость от создания папки plugins
add_dependencies(calc create_plugins_dir)

# Для Windows (LoadLibrary, GetProcAddress)
target_link_libraries(calc Kernel32)

# Требуем C++17 для filesystem
if(MSVC)
    target_compile_features(calc PRIVATE cxx_std_17)
else()
    target_link_libraries(calc stdc++fs)
endif()