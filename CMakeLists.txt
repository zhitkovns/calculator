cmake_minimum_required(VERSION 3.15)
project(Calculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

# Создаем папку plugins если ее нет
add_custom_target(create_plugins_dir ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/plugins
    COMMENT "Creating plugins directory"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
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
    src/operations/wrappers/
    src/plugins/
)

add_executable(calc ${MAIN_SRC_FILES})
add_dependencies(calc create_plugins_dir)

# Устанавливаем выходную директорию для основного приложения
set_target_properties(calc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)

if(MSVC)
    target_compile_features(calc PRIVATE cxx_std_17)
    target_link_libraries(calc Kernel32)
else()
    target_link_libraries(calc stdc++fs)
endif()

# Подключаем сборку плагинов
add_subdirectory(src/extensions)

# Тесты
message(STATUS "Building tests")

set(TEST_SRC_FILES
    tests/main.cpp
    tests/unit/TestCalculator.cpp
    tests/unit/TestExpressionParser.cpp
    tests/unit/TestAST.cpp
    tests/unit/TestBuiltinOperations.cpp
    tests/unit/TestErrorHandling.cpp
    tests/integration/TestFullExpressions.cpp
    tests/integration/TestPlugins.cpp
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

add_executable(tests ${TEST_SRC_FILES})
add_dependencies(tests create_plugins_dir)

target_include_directories(tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/tests
)

# Устанавливаем выходную директорию для тестов
set_target_properties(tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)

if(MSVC)
    target_compile_features(tests PRIVATE cxx_std_17)
    target_link_libraries(tests Kernel32)
else()
    target_link_libraries(tests stdc++fs pthread)
endif()