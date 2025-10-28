set(PLUGINS_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/plugins)

# Cos extension
add_library(func_cos SHARED CosExtension/CosExtension.cpp)
set_target_properties(func_cos PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PLUGINS_OUTPUT_DIR}
    OUTPUT_NAME "_c"
)

# Sin extension
add_library(func_sin SHARED SinExtension/SinExtension.cpp)
set_target_properties(func_sin PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PLUGINS_OUTPUT_DIR}
    OUTPUT_NAME "_s"
)