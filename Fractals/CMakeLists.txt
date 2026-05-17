set(PROJECT_NAME "Fractals")

# Adding executable
file(GLOB_RECURSE
    SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

file(GLOB_RECURSE
    HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
