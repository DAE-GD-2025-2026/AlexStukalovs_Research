get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

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
target_link_libraries(${PROJECT_NAME} PRIVATE Engine D0L_LIB)

# Copying Resources/ to the project's build directory on build
set(RESOURCE_DIR_NAME "Resources")
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/${RESOURCE_DIR_NAME}"
    "${CMAKE_CURRENT_BINARY_DIR}/${RESOURCE_DIR_NAME}"
    VERBATIM
)
