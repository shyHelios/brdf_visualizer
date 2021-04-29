cmake_minimum_required(VERSION 3.17)
project(BRDFVizProject)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Where to find CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# Enable export of compile commands for PVS studio
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Setup path to libs
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

option(USE_ANALYSER OFF)

# Add embree to project
include(${CMAKE_MODULE_PATH}/embree.cmake)
# Add glm to project
include(${CMAKE_MODULE_PATH}/glm.cmake)
# Add glfw to project
include(${CMAKE_MODULE_PATH}/glfw3.cmake)
# Add glad to project
include(${CMAKE_MODULE_PATH}/glad.cmake)
if (USE_ANALYSER)
  # Add PVS studio to project
  include(${CMAKE_MODULE_PATH}/PVS-Studio.cmake)
endif ()
# Add imgui to project
include(${CMAKE_MODULE_PATH}/imgui.cmake)
# Add assimp to project
include(${CMAKE_MODULE_PATH}/assimp.cmake)
# Add spdlog to project
include(${CMAKE_MODULE_PATH}/spdlog.cmake)
# Add doxygen to project
include(${CMAKE_MODULE_PATH}/doxygen.cmake)

add_subdirectory(src)

