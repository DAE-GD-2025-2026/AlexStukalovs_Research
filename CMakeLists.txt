cmake_minimum_required(VERSION 4.1)

# Setting the name of the directory as the name of the project
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Common dependencies
Include(FetchContent)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/D0L")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Fractals")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Tree2D")
