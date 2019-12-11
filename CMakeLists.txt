# revisit this! Find a nicer way to handle unknown CMake versions
cmake_minimum_required(VERSION 3.12)

project(DSSHomework VERSION 1.0
    DESCRIPTION "Implements the DSS take-home test"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(DSSHomework src/main.cpp)
find_library(ws2_32 REQUIRED)
target_link_libraries(DSSHomework ws2_32)
# Our headers
target_include_directories(DSSHomework PUBLIC include)

# Add on the third-party, header-only deps
# Wrap them in my own custom library targets to make it easier to target them in a normal CMake way

## CPP HTTP LIB
add_library(CppHttpLib INTERFACE)
# NOTE: Adding "SYSTEM" is kind of a hack to suppress warnings, otherwise we get one warning which is no fun
target_include_directories(CppHttpLib SYSTEM INTERFACE thirdparty/cpp-httplib)

target_link_libraries(DSSHomework CppHttpLib)

## JSON