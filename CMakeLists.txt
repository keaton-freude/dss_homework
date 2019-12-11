# revisit this! Find a nicer way to handle unknown CMake versions
cmake_minimum_required(VERSION 3.12)

project(DSSHomework VERSION 1.0
    DESCRIPTION "Implements the DSS take-home test"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(DSSHomework src/main.cpp)

# Our headers
target_include_directories(DSSHomework PUBLIC include)

# Add on the third-party, header-only deps

# Wrap them in my own custom library targets to make it easier to target them in a normal CMake way
## CPP HTTP LIB
add_library(CppHttpLib INTERFACE)
target_include_directories(CppHttpLib INTERFACE thirdparty/cpp-httplib)

target_link_libraries(DSSHomework CppHttpLib)

## JSON
add_library(JsonLib INTERFACE)
target_include_directories(JsonLib INTERFACE thirdparty/json)

target_link_libraries(DSSHomework JsonLib)

## GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(thirdparty/glfw-3.3)

target_link_libraries(DSSHomework glfw)

## OpenGL
find_package(OpenGL REQUIRED)

target_include_directories(DSSHomework PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(DSSHomework ${OPENGL_gl_LIBRARY})

## GLM
add_library(GlmLib INTERFACE)
target_include_directories(GlmLib INTERFACE thirdparty/glm)

target_link_libraries(DSSHomework GlmLib)