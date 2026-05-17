set(PROJECT_NAME "D0L")

# Managing dependencies
Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.15.0
)

FetchContent_MakeAvailable(Catch2)

enable_testing()

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

# Adding library for the tested code
add_library(D0L_LIB STATIC
    D0L.hpp
    D0L.cpp
)
target_include_directories(D0L_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Adding executable
set(SOURCES
    Main.cpp
)

file(GLOB_RECURSE
    HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} PRIVATE D0L_LIB)

add_subdirectory(Tests)
