cmake_minimum_required(VERSION 4.1)

# Setting the name of the directory as the name of the project
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Common dependencies
Include(FetchContent)

add_subdirectory("Engine")
# Adding all projects
set(PROJECT_DIR_NAME "Projects")
file(GLOB SUBDIRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_DIR_NAME}" "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_DIR_NAME}/*")
foreach(SUBDIR ${SUBDIRS})
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_DIR_NAME}/${SUBDIR}")
        add_subdirectory("${PROJECT_DIR_NAME}/${SUBDIR}")
    endif()
endforeach()
