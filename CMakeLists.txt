# revisit this! Find a nicer way to handle unknown CMake versions
cmake_minimum_required(VERSION 3.12)

project(DSSHomework VERSION 1.0
    DESCRIPTION "Implements the DSS take-home test"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(DSSHomework
    thirdparty/imgui/examples/imgui_impl_opengl3.cpp
    thirdparty/imgui/examples/imgui_impl_glfw.cpp
    thirdparty/imgui/imgui_draw.cpp
    thirdparty/imgui/imgui_widgets.cpp
    thirdparty/imgui/imgui.cpp
    src/HTTPSClient.cpp
    src/MLBStatsFetcher.cpp
    src/Text.cpp
    src/ContentTileList.cpp
    src/ContentTile.cpp
    src/Texture.cpp
    src/ShaderProgram.cpp
    src/Background.cpp
    src/QuadMesh.cpp
    src/VertexBuffer.cpp
    src/IndexBuffer.cpp
    src/Input.cpp
    src/Application.cpp
    src/Window.cpp
    src/main.cpp)

# Not totally necessary, but I like having the console window when I'm not running
# from a shell. If we run from a shell, we get output sent to that shell
if (MSVC)
    set_target_properties(DSSHomework PROPERTIES
    LINK_FLAGS /SUBSYSTEM:CONSOLE
    )
endif()

# Our headers
target_include_directories(DSSHomework PUBLIC include)


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

## GLEW
add_subdirectory(thirdparty/glew-2.1.0/build/cmake)
target_include_directories(DSSHomework PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/glew-2.1.0/include)
target_link_libraries(DSSHomework glew)

## STB Image
target_include_directories(DSSHomework PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/stb)

## GLText
target_include_directories(DSSHomework PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/gltext)

## ImGUI
target_include_directories(DSSHomework PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/imgui)
target_include_directories(DSSHomework PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/imgui/examples)

## URL 
# Code taken from this Stack Overflow answer: https://stackoverflow.com/a/2616217
target_include_directories(DSSHomework PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/url)

## Poco Stuff
target_include_directories(DSSHomework PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/poco/include)

# Link each individual required library...
target_link_libraries(DSSHomework ${PROJECT_SOURCE_DIR}/thirdparty/poco/lib/libcryptod.lib)
target_link_libraries(DSSHomework ${PROJECT_SOURCE_DIR}/thirdparty/poco/lib/libssld.lib)
target_link_libraries(DSSHomework ${PROJECT_SOURCE_DIR}/thirdparty/poco/lib/PocoCryptod.lib)
target_link_libraries(DSSHomework ${PROJECT_SOURCE_DIR}/thirdparty/poco/lib/PocoFoundationd.lib)
target_link_libraries(DSSHomework ${PROJECT_SOURCE_DIR}/thirdparty/poco/lib/PocoJSONd.lib)
target_link_libraries(DSSHomework ${PROJECT_SOURCE_DIR}/thirdparty/poco/lib/PocoNetd.lib)
target_link_libraries(DSSHomework ${PROJECT_SOURCE_DIR}/thirdparty/poco/lib/PocoNetSSLd.lib)
target_link_libraries(DSSHomework ${PROJECT_SOURCE_DIR}/thirdparty/poco/lib/PocoUtild.lib)
target_link_libraries(DSSHomework ${PROJECT_SOURCE_DIR}/thirdparty/poco/lib/PocoXMLd.lib)

file(GLOB POCO_RUNTIME_LIBRARIES
    ${CMAKE_CURRENT_LIST_DIR}/thirdparty/poco/bin/*.dll
)

# Copy all required resources (graphics resources, certs, poco runtime DLLs) next to the binary
add_custom_command(TARGET DSSHomework POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${POCO_RUNTIME_LIBRARIES} $<TARGET_FILE_DIR:DSSHomework>
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:DSSHomework>/resources
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/thirdparty/openssl/cacert.pem $<TARGET_FILE_DIR:DSSHomework>
    )

set_target_properties(DSSHomework PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build/bin/)