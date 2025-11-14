cmake_minimum_required(VERSION 3.19)

project(hatch_generator CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXEC_SOURCE 
    src/main.cpp
    src/geometry.cpp
    src/cmdline_parser.cpp
    src/svg_writer.cpp
)

add_executable(${PROJECT_NAME} ${EXEC_SOURCE})
target_include_directories(${PROJECT_NAME} PRIVATE include)