cmake_minimum_required(VERSION 3.15)
project(Calculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Создаем папку extensions
add_custom_target(create_extensions_dir ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/extensions
)

# Основное приложение
set(MAIN_SRC_FILES
    src/main.cpp
    src/core/Calculator.cpp
    src/core/ExpressionParser.cpp
    src/core/OperationFactory.cpp
    src/core/AST/NumberNode.cpp
    src/core/AST/BinaryNode.cpp
    src/core/AST/UnaryNode.cpp
    src/core/AST/FunctionNode.cpp
    src/operations/BuiltinOperations.cpp
    src/operations/wrappers/ExtensionOperationWrapper.cpp
    src/plugins/ExtensionRegistry.cpp
    src/plugins/ExtensionUnit.cpp
    src/plugins/HostIntegration.cpp
    src/plugins/DynamicLibrary.cpp
)

include_directories(
    src/
    src/core/
    src/core/AST/
    src/operations/
    src/plugins/
)

add_executable(calc ${MAIN_SRC_FILES})
add_dependencies(calc create_extensions_dir)
target_link_libraries(calc Kernel32)

if(MSVC)
    target_compile_features(calc PRIVATE cxx_std_17)
else()
    target_link_libraries(calc stdc++fs)
endif()

# Подключаем сборку плагинов
add_subdirectory(src/extensions)