cmake_minimum_required(VERSION 3.10)
project(GraphicsEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(engine ${SOURCES})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)


target_link_libraries(engine PUBLIC 
    GLEW::GLEW
    glfw
    ${OPENGL_LIBRARIES}
)

target_include_directories(engine PRIVATE include)