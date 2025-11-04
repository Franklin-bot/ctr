#include "../../src/parser/nfa.hpp"

#include <cwchar>
#include <gtest/gtest.h>


TEST(ConcatentationTest, Atoms){

    ctr::string_t pattern{"ab"};
    EXPECT_EQ(ctr::NFA::InsertConcat(pattern), "a&b");

    pattern = "a4";
    EXPECT_EQ(ctr::NFA::InsertConcat(pattern), "a&4");

    pattern = "a*";
    EXPECT_EQ(ctr::NFA::InsertConcat(pattern), "a*");
}

TEST(ConcatentationTest, Groups){

    ctr::string_t pattern{"a(b|a)"};
    EXPECT_EQ(ctr::NFA::InsertConcat(pattern), "a&(b|a)");

    pattern = "(a|b)(c|d)";
    EXPECT_EQ(ctr::NFA::InsertConcat(pattern), "(a|b)&(c|d)");

    pattern = "(c|d)a*";
    EXPECT_EQ(ctr::NFA::InsertConcat(pattern), "(c|d)&a*");

    pattern = "a*(c*(b|d))a*";
    EXPECT_EQ(ctr::NFA::InsertConcat(pattern), "a*&(c*&(b|d))&a*");
}

TEST(ShuntingYardTest, Atoms){

    ctr::string_t pattern = ctr::NFA::InsertConcat("ab");
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern), "ab&");

    ctr::string_t pattern2 = ctr::NFA::InsertConcat("ab|c");
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern2), "ab&c|");

}

TEST(ShuntingYardTest, SingleLiteral){
    ctr::string_t pattern = ctr::NFA::InsertConcat("a");
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern), "a");
}

TEST(ShuntingYardTest, SimpleConcat){
    ctr::string_t pattern = ctr::NFA::InsertConcat("ab");
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern), "ab&");

    ctr::string_t pattern2 = ctr::NFA::InsertConcat("abc");
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern2), "ab&c&");
}

TEST(ShuntingYardTest, Alternation){
    ctr::string_t pattern = ctr::NFA::InsertConcat("a|b");
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern), "ab|");

    ctr::string_t pattern2 = ctr::NFA::InsertConcat("a|b|c");
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern2), "ab|c|");
}

TEST(ShuntingYardTest, UnaryOperators){
    ctr::string_t pattern1 = ctr::NFA::InsertConcat("a*");
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern1), "a*");
}

TEST(ShuntingYardTest, ConcatWithUnary){
    ctr::string_t pattern1 = ctr::NFA::InsertConcat("ab*");
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern1), "ab*&");
}

TEST(ShuntingYardTest, ParenthesesAndConcat){
    ctr::string_t pattern1 = ctr::NFA::InsertConcat("(ab)");
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern1), "ab&");

    ctr::string_t pattern2 = ctr::NFA::InsertConcat("(ab)*");
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern2), "ab&*");

    ctr::string_t pattern3 = ctr::NFA::InsertConcat("a(b|c)d");
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern3), "abc|&d&");
}

TEST(ShuntingYardTest, Precedence){
    // '*' > '&' > '|'
    ctr::string_t pattern1 = ctr::NFA::InsertConcat("a|b*c");
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern1), "ab*c&|");

    ctr::string_t pattern2 = ctr::NFA::InsertConcat("(a|b)*c");
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern2), "ab|*c&");

    ctr::string_t pattern3 = ctr::NFA::InsertConcat("a(b|c)*d");
    ctr::print(pattern3);
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern3), "abc|*&d&");
}

TEST(ShuntingYardTest, NestedAndMixed){
    ctr::string_t pattern1 = ctr::NFA::InsertConcat("a|(bc)d");
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern1), "abc&d&|");

    ctr::string_t pattern2 = ctr::NFA::InsertConcat("a(bc)|d");
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern2), "abc&&d|");

    ctr::string_t pattern3 = ctr::NFA::InsertConcat("(a|b)(c|d)");
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern3), "ab|cd|&");
}

TEST(ShuntingYardTest, LongerExpression){
    // ((a|b)&(c|d)*)&e|f  with implicit concats
    ctr::string_t pattern = ctr::NFA::InsertConcat("(a|b)(c|d)*e|f");
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern), "ab|cd|*&e&f|");
}


int main(int argc, char **argv) {

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}




