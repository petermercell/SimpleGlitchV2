# Container
if(DEFINED ENV{GLOBAL_TOOLCHAIN})
    message("Set toolchain $ENV{GLOBAL_TOOLCHAIN}")
    set(CMAKE_TOOLCHAIN_FILE $ENV{GLOBAL_TOOLCHAIN})
endif()

# CMake
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(SimpleGlitch2)

cmake_policy(SET CMP0074 NEW)

# choose "Delete Cache and Reconfigure"
# set(Nuke_ROOT "C:/Program Files/Nuke13.1v5") # manual

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Nuke REQUIRED)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if (NUKE_VERSION_MAJOR VERSION_GREATER_EQUAL 15)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_USE_CXX11_ABI=1")
    set(CMAKE_CXX_STANDARD 17)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_USE_CXX11_ABI=0")
    set(CMAKE_CXX_STANDARD 14)
endif()

# add sub directory
add_subdirectory(src)