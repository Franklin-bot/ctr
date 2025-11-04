set(SRC_FILES
    nfa.hpp
    nfa.cpp
    automaton.hpp
)

add_library(parser STATIC ${SRC_FILES})

target_link_libraries(parser PRIVATE common)
