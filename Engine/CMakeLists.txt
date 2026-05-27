get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# Adding library
file(GLOB_RECURSE
    SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

file(GLOB_RECURSE
    HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Include")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Include/Engine")


target_link_libraries(${PROJECT_NAME} PUBLIC
    D0L_LIB
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
)

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})



# Copying SDL dll files to the engine's build directory
foreach (DLL_TARGET IN LISTS SDL_LIBS)
    add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:${DLL_TARGET}>"
            ${CMAKE_CURRENT_BINARY_DIR}
            VERBATIM
    )
endforeach ()
