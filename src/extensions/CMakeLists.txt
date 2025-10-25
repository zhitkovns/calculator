# Build mathematical extensions

# Sin function extension
add_library(func_sin SHARED SinExtension/SinExtension.cpp)
target_include_directories(func_sin PRIVATE ../plugins)

if(MSVC)
    target_compile_options(func_sin PRIVATE "/W4")
endif()

target_link_libraries(func_sin)

# Output to extensions folder with correct name
set_target_properties(func_sin PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/extensions"
    OUTPUT_NAME "func_sin"
    PREFIX ""
    SUFFIX ""
)