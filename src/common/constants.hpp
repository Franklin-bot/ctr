#pragma once

#include "encoding.hpp"

#include <cctype>
#include <cstddef>
#include <string>

namespace ctr{


    struct RegexOp {
        char_t op;
        int precedence = 0;
        bool left_assoc = false;

        bool operator== (const RegexOp& other) const {
            return op == other.op && precedence == other.precedence;
        }

        bool operator!= (const RegexOp& other) const {
            return op != other.op || precedence != other.precedence;
        }
    };

    static constexpr RegexOp Concat{U'&', 2};
    static constexpr RegexOp Alternation{U'|', 1};
    static constexpr RegexOp Kleene{U'*', 3, false};
    static constexpr RegexOp NewGroup{U'(', 0};
    static constexpr RegexOp CloseGroup{U')', 0};

    static RegexOp GetRegexOp(char_t c){
        switch(c){
            case U'&':
                return Concat;
            case U'|':
                return Alternation;
            case U'*':
                return Kleene;
            case U'(':
                return NewGroup;
            case U')':
                return CloseGroup;
            default:
                throw std::invalid_argument("Input char must be a regex op");
        }

    }

    static bool IsAtom(char_t c){
        return std::iswalnum(c) || c == U'.';
    }

    static bool IsQuantifier(char_t c){
        return c == Kleene.op;
    }

    static bool IsNewGroup(char_t c){
        return c == NewGroup.op;
    }

    static bool IsClosedGroup(char_t c){
        return c == CloseGroup.op;
    }
};
