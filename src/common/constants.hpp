#pragma once

#include "encoding.hpp"

#include <cctype>
#include <cstddef>

namespace ctr{

    static constexpr char_t Concat = U'·';
    static constexpr char_t Alternation = U'|';
    static constexpr char_t Kleene = U'*';
    static constexpr char_t NewGroup = U'(';
    static constexpr char_t CloseGroup = U')';
    static constexpr char_t Epsilon = U'ε';
    static constexpr char_t Empty = U'∅';


    struct RegexOp {
        char_t op;
        int precedence = 0;
        bool left_assoc = true;

        bool operator== (const RegexOp& other) const {
            return op == other.op && precedence == other.precedence;
        }

        bool operator!= (const RegexOp& other) const {
            return op != other.op || precedence != other.precedence;
        }
    };

    static constexpr RegexOp ConcatOp{Concat, 2};
    static constexpr RegexOp AlternationOp{Alternation, 1};
    static constexpr RegexOp KleeneOp{Kleene, 3, false};
    static constexpr RegexOp NewGroupOp{NewGroup, 0};
    static constexpr RegexOp CloseGroupOp{CloseGroup, 0};

    static RegexOp GetRegexOp(char_t c){
        switch(c){
            case Concat:
                return ConcatOp;
            case Alternation:
                return AlternationOp;
            case Kleene:
                return KleeneOp;
            case NewGroup:
                return NewGroupOp;
            case CloseGroup:
                return CloseGroupOp;
            default:
                throw std::invalid_argument("Input char must be a regex op");
        }

    }

    static bool IsRegexOp(char_t c){
        return c == ConcatOp.op || c == AlternationOp.op || c == KleeneOp.op;
    }

    static bool IsAtom(char_t c){
        return std::iswalnum(c) || c == U'.';
    }

    static bool IsQuantifier(char_t c){
        return c == KleeneOp.op;
    }

    static bool IsNewGroup(char_t c){
        return c == NewGroupOp.op;
    }

    static bool IsClosedGroup(char_t c){
        return c == CloseGroupOp.op;
    }
};
