cmake_minimum_required(VERSION 3.15)
project(Calculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Создаем папку plugins в папке проекта
add_custom_target(create_plugins_dir ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/plugins
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
target_link_libraries(calc Kernel32)

if(MSVC)
    target_compile_features(calc PRIVATE cxx_std_17)
else()
    target_link_libraries(calc stdc++fs)
endif()

# Подключаем сборку плагинов
add_subdirectory(src/extensions)




# Опция для включения тестов
option(BUILD_TESTS "Build tests" ON)

# Создаем tests.exe как обычную цель
if(BUILD_TESTS)
    # Сначала пробуем найти системный GTest
    find_package(GTest QUIET)
    
    if(NOT GTest_FOUND)
        # Если системного нет, используем локальную папку
        if(EXISTS ${CMAKE_SOURCE_DIR}/googletest/CMakeLists.txt)
            message(STATUS "Using bundled Google Test")
            add_subdirectory(googletest)
            
            # Создаем псевдо-цели для совместимости
            if(NOT TARGET GTest::gtest)
                add_library(GTest::gtest ALIAS gtest)
            endif()
            if(NOT TARGET GTest::gtest_main)
                add_library(GTest::gtest_main ALIAS gtest_main)
            endif()
        else()
            message(WARNING "Google Test not found. Please install GTest or clone googletest in project root")
            return()
        endif()
    else()
        message(STATUS "Using system Google Test")
    endif()
    
    find_package(Threads REQUIRED)
    
    # Исходные файлы основной программы для тестов
    set(MAIN_SRC_FILES
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
    
    # Тестовые файлы
    set(TEST_SRC_FILES
        tests/main.cpp
        tests/unit/TestCalculator.cpp
        tests/unit/TestExpressionParser.cpp
        tests/unit/TestAST.cpp
        tests/unit/TestBuiltinOperations.cpp
        tests/integration/TestFullExpressions.cpp
    )
    
    # Исполняемый файл тестов
    add_executable(tests ${MAIN_SRC_FILES} ${TEST_SRC_FILES})
    
    # Директории для include
    target_include_directories(tests PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/tests
    )
    
    # Линковка
    target_link_libraries(tests
        GTest::gtest
        GTest::gtest_main
        Threads::Threads
    )
    
    if(MSVC)
        target_compile_features(tests PRIVATE cxx_std_17)
        target_link_libraries(tests Kernel32)
    else()
        target_link_libraries(tests stdc++fs)
    endif()
    
    # Устанавливаем выходную директорию в корень проекта
    set_target_properties(tests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    
    message(STATUS "Tests target 'tests' configured")
endif()