add_executable(D0L_TESTS
        Tests.cpp
)

target_link_libraries(D0L_TESTS PRIVATE
    D0L_LIB
    Catch2::Catch2WithMain
)

catch_discover_tests(D0L_TESTS)
