cmake_minimum_required(VERSION 3.29)

# Setting the name of the directory as the name of the project
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Common dependencies
include(FetchContent)

# Dependenciesa

# Requesting SDL_ttf to not try using system libraries
set(SDLTTF_BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
unset(FREETYPE_LIBRARY CACHE)
unset(FREETYPE_INCLUDE_DIRS CACHE)
set(SDLTTF_VENDORED ON CACHE BOOL "" FORCE)
set(SDLTTF_FREETYPE_VENDORED ON CACHE BOOL "" FORCE)
set(SDLTTF_HARFBUZZ_VENDORED ON CACHE BOOL "" FORCE)

FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.4.0
        # GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE# Displaying cloning process in terminal
)

#FetchContent_Declare(
#    SDL3_ttf
#    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
#    GIT_TAG release-3.2.2
#    #GIT_SHALLOW TRUE # Need to fetch submodules
#    GIT_PROGRESS TRUE
#)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
        SDL3_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
        GIT_TAG release-3.2.2
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
set(SDL3TTF_INSTALL OFF)
FetchContent_MakeAvailable(SDL3_ttf)

set(SDL_LIBS
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
)

add_subdirectory("Engine")

# Adding all projects
set(PROJECT_DIR_NAME "Projects")
file(GLOB SUBDIRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_DIR_NAME}" "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_DIR_NAME}/*")
foreach(SUBDIR ${SUBDIRS})
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_DIR_NAME}/${SUBDIR}")
        add_subdirectory("${PROJECT_DIR_NAME}/${SUBDIR}")
    endif()
endforeach()
