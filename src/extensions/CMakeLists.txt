set(PLUGINS_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/plugins)

# Cos extension
add_library(func_cos SHARED CosExtension/CosExtension.cpp)
set_target_properties(func_cos PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PLUGINS_OUTPUT_DIR}
    OUTPUT_NAME "_cos"
)

# Sin extension
add_library(func_sin SHARED SinExtension/SinExtension.cpp)
set_target_properties(func_sin PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PLUGINS_OUTPUT_DIR}
    OUTPUT_NAME "_sin"
)

# Logarithm extension
add_library(func_ln SHARED LogarithmExtension/LogarithmExtension.cpp)
set_target_properties(func_ln PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PLUGINS_OUTPUT_DIR}
    OUTPUT_NAME "_ln"
)

# UnaryMinus extension
add_library(func_unary SHARED UnaryMinusExtension/UnaryMinusExtension.cpp)
set_target_properties(func_unary PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PLUGINS_OUTPUT_DIR}
    OUTPUT_NAME "_unary"
)

# Power extension
add_library(func_pow SHARED PowerExtension/PowerExtension.cpp)
set_target_properties(func_pow PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PLUGINS_OUTPUT_DIR}
    OUTPUT_NAME "_pow"
)