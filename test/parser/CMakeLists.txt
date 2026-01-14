find_package(GTest REQUIRED)

add_executable(parser_tests parser_test.cpp)
target_link_libraries(parser_tests PRIVATE gtest::gtest)
add_test(NAME parsing_tests COMMAND parser_tests)

add_executable(nfa_tests nfa_test.cpp)
target_link_libraries(nfa_tests PRIVATE gtest::gtest)
add_test(NAME nfa_tests COMMAND nfa_tests)

