cmake_minimum_required (VERSION 2.8)
# Project's name


project(simple-wx-model)

# Set output folder
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}")
add_executable(simple-wx-model ${PROJECT_SOURCE_DIR}/main.cpp)
target_link_libraries(simple-wx-model GLFW)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(simple-wx-model ${OPENGL_LIBRARIES})
