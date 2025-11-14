

add_executable(parser_tests parser_test.cpp)
add_executable(nfa_tests nfa_test.cpp)

find_package(GTest REQUIRED)
target_link_libraries(parser_tests PRIVATE parser gtest::gtest)
target_link_libraries(nfa_tests PRIVATE parser gtest::gtest)
