set(ENGINE_NAME ENGINE-3D)
file(GLOB_RECURSE FILE_SOURCE src/*.cpp glad/src/*.c)

cmake_minimum_required(VERSION 3.20)

project(${ENGINE_NAME} LANGUAGES CXX C)

add_subdirectory(glfw/)
add_executable(${ENGINE_NAME} ${FILE_SOURCE})

target_link_libraries(${ENGINE_NAME} glfw)

include_directories(glfw/include glad/include glm src/)
