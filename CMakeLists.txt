set(ENGINE_NAME ENGINE-3D)

file(GLOB_RECURSE FILE_SOURCE src/*.cpp glad/src/*.c)
file(GLOB FILE_SOURCE ${FILE_SOURCE} 
        imgui/*.cpp imgui/*.h 
        imgui/backends/imgui_impl_opengl3.cpp 
        imgui/backends/imgui_impl_glfw.cpp 
        imgui/misc/cpp/imgui_stdlib.cpp)

cmake_minimum_required(VERSION 3.20)

project(${ENGINE_NAME} LANGUAGES CXX C)

add_subdirectory(glfw/)
add_executable(${ENGINE_NAME} ${FILE_SOURCE})

target_link_libraries(${ENGINE_NAME} glfw)

include_directories(glfw/include glad/include imgui/ imgui/backends glm src/)

file(REMOVE ${CMAKE_BINARY_DIR}/res)
file(COPY res DESTINATION ${CMAKE_BINARY_DIR})

install(TARGETS ${ENGINE_NAME} DESTINATION bin)
install(DIRECTORY res DESTINATION bin)
