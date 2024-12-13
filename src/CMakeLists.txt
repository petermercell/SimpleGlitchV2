# Define el plugin a compilar
set(TARGET_PLUGIN "SimpleGlitch2")  # Cambia "DeepCAdd" por el nombre de tu plugin

# Incluir directorios necesarios
include_directories(${NUKE_INCLUDE_DIRS})

# Crear el plugin seleccionado
add_library(${TARGET_PLUGIN} MODULE SimpleGlitch.cpp)
add_library(NukePlugins::${TARGET_PLUGIN} ALIAS ${TARGET_PLUGIN})
target_link_libraries(${TARGET_PLUGIN} PRIVATE ${NUKE_DDIMAGE_LIBRARY})

if (NUKE_VERSION_MAJOR VERSION_GREATER_EQUAL 14.0)
    target_compile_definitions(SimpleGlitch2 PRIVATE NOMINMAX _USE_MATH_DEFINES)
endif()

# Propiedades específicas para MacOS
if (APPLE)
    set_target_properties(${TARGET_PLUGIN} PROPERTIES SUFFIX ".dylib")
endif()

# Instalación del plugin
install(TARGETS ${TARGET_PLUGIN} DESTINATION .)
