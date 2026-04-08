add_executable(woh_mod_creator_tests
    test_smoke.cpp
    ito_document_tests.cpp
    ito_parser_tests.cpp
)

target_link_libraries(woh_mod_creator_tests
    PRIVATE
        GTest::gtest_main
        woh_core
        woh_ito
)

target_compile_features(woh_mod_creator_tests PRIVATE cxx_std_20)

include(GoogleTest)
gtest_discover_tests(woh_mod_creator_tests)
