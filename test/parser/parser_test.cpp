#include "../../src/parser/nfa.hpp"

#include <gtest/gtest.h>


TEST(ConcatentationTest, Atoms){

    ctr::string_t pattern{U"ab"};
    EXPECT_EQ(ctr::NFA::InsertConcat(pattern), U"a·b");

    pattern = U"a4";
    EXPECT_EQ(ctr::NFA::InsertConcat(pattern), U"a·4");

    pattern = U"a*";
    EXPECT_EQ(ctr::NFA::InsertConcat(pattern), U"a*");
}

TEST(ConcatentationTest, Groups){

    ctr::string_t pattern{U"a(b|a)"};
    EXPECT_EQ(ctr::NFA::InsertConcat(pattern), U"a·(b|a)");

    pattern = U"(a|b)(c|d)";
    EXPECT_EQ(ctr::NFA::InsertConcat(pattern), U"(a|b)·(c|d)");

    pattern = U"(c|d)a*";
    EXPECT_EQ(ctr::NFA::InsertConcat(pattern), U"(c|d)·a*");

    pattern = U"a*(c*(b|d))a*";
    EXPECT_EQ(ctr::NFA::InsertConcat(pattern), U"a*·(c*·(b|d))·a*");
}

TEST(ShuntingYardTest, Atoms){

    ctr::string_t pattern = U"ab";
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern), U"ab·");

    ctr::string_t pattern2 = ctr::NFA::InsertConcat(U"ab|c");
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern2), U"ab·c|");

}

TEST(ShuntingYardTest, SingleLiteral){
    ctr::string_t pattern = U"a";
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern), U"a");
}

TEST(ShuntingYardTest, SimpleConcat){
    ctr::string_t pattern = U"ab";
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern), U"ab·");

    ctr::string_t pattern2 = U"abc";
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern2), U"ab·c·");
}

TEST(ShuntingYardTest, Alternation){
    ctr::string_t pattern = U"a|b";
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern), U"ab|");

    ctr::string_t pattern2 = U"a|b|c";
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern2), U"ab|c|");
}

TEST(ShuntingYardTest, UnaryOperators){
    ctr::string_t pattern1 = U"a*";
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern1), U"a*");
}

TEST(ShuntingYardTest, ConcatWithUnary){
    ctr::string_t pattern1 = U"ab*";
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern1), U"ab*·");
}

TEST(ShuntingYardTest, ParenthesesAndConcat){
    ctr::string_t pattern1 = U"(ab)";
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern1), U"ab·");

    ctr::string_t pattern2 = U"(ab)*";
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern2), U"ab·*");

    ctr::string_t pattern3 = U"a(b|c)d";
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern3), U"abc|·d·");
}

TEST(ShuntingYardTest, Precedence){
    ctr::string_t pattern1 = U"a|b*c";
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern1), U"ab*c·|");

    ctr::string_t pattern2 = U"(a|b)*c";
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern2), U"ab|*c·");

    ctr::string_t pattern3 = U"a(b|c)*d";
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern3), U"abc|*·d·");
}

TEST(ShuntingYardTest, NestedAndMixed){
    ctr::string_t pattern1 = U"a|(bc)d";
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern1), U"abc·d·|");

    ctr::string_t pattern2 = U"a(bc)|d";
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern2), U"abc··d|");

    ctr::string_t pattern3 = U"(a|b)(c|d)";
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern3), U"ab|cd|·");
}

TEST(ShuntingYardTest, LongerExpression){
    ctr::string_t pattern = U"(a|b)(c|d)*e|f";
    EXPECT_EQ(ctr::NFA::ShuntingYard(pattern), U"ab|cd|*·e·f|");
}


int main(int argc, char **argv) {

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}




