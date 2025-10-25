# Сборка плагинов в папку plugins проекта (не в build)
set(PLUGINS_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/plugins)

# Sine extension
add_library(func_sin SHARED SinExtension/SinExtension.cpp)
set_target_properties(func_sin PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PLUGINS_OUTPUT_DIR}
    OUTPUT_NAME "sin_plugin"  # Имя файла можно задать явно
)

# Добавьте другие плагины по аналогии:
# add_library(func_cos SHARED CosineExtension/CosineExtension.cpp)
# set_target_properties(func_cos PROPERTIES
#     RUNTIME_OUTPUT_DIRECTORY ${PLUGINS_OUTPUT_DIR}
#     OUTPUT_NAME "cos_plugin"
# )