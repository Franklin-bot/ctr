#include "../../src/common/fixed_string.hpp"
#include "../../src/common/pure/algos.hpp"

#include <gtest/gtest.h>

#include <type_traits>

TEST(ShuntingYardTest, Atoms)
{
  static_assert(
    std::is_same_v<ctr::ShuntingYard<ctr::fixed_string{"A"}>, ctr::deque<ctr::ch<'A'>>>
  );

  static_assert(
    std::is_same_v<ctr::ShuntingYard<ctr::fixed_string{"Z"}>, ctr::deque<ctr::ch<'Z'>>>
  );
}

TEST(ShuntingYardTest, Union_Basic)
{
  static_assert(std::is_same_v<
                ctr::ShuntingYard<ctr::fixed_string{"A|B"}>,
                ctr::deque<ctr::ch<'A'>, ctr::ch<'B'>, ctr::ch<'|'>>>);

  static_assert(std::is_same_v<
                ctr::ShuntingYard<ctr::fixed_string{"X|Y"}>,
                ctr::deque<ctr::ch<'X'>, ctr::ch<'Y'>, ctr::ch<'|'>>>);
}

TEST(ShuntingYardTest, Parentheses_UnionOnly)
{
  static_assert(std::is_same_v<
                ctr::ShuntingYard<ctr::fixed_string{"(A|B)"}>,
                ctr::deque<ctr::ch<'A'>, ctr::ch<'B'>, ctr::ch<'|'>>>);

  static_assert(std::is_same_v<
                ctr::ShuntingYard<ctr::fixed_string{"((A|B))"}>,
                ctr::deque<ctr::ch<'A'>, ctr::ch<'B'>, ctr::ch<'|'>>>);
}

TEST(ShuntingYardTest, Star_Basic)
{
  static_assert(std::is_same_v<
                ctr::ShuntingYard<ctr::fixed_string{"A*"}>,
                ctr::deque<ctr::ch<'A'>, ctr::ch<'*'>>>);

  static_assert(std::is_same_v<
                ctr::ShuntingYard<ctr::fixed_string{"Z*"}>,
                ctr::deque<ctr::ch<'Z'>, ctr::ch<'*'>>>);
}

TEST(ShuntingYardTest, Star_WithParentheses)
{
  // (A|B)* => A B | *
  static_assert(std::is_same_v<
                ctr::ShuntingYard<ctr::fixed_string{"(A|B)*"}>,
                ctr::deque<ctr::ch<'A'>, ctr::ch<'B'>, ctr::ch<'|'>, ctr::ch<'*'>>>);

  static_assert(std::is_same_v<
                ctr::ShuntingYard<ctr::fixed_string{"((A|B))*"}>,
                ctr::deque<ctr::ch<'A'>, ctr::ch<'B'>, ctr::ch<'|'>, ctr::ch<'*'>>>);
}

TEST(ShuntingYardTest, Union_Associativity_Sanity)
{
  // union typically left associative: (A|B)|C
  static_assert(
    std::is_same_v<
      ctr::ShuntingYard<ctr::fixed_string{"A|B|C"}>,
      ctr::deque<ctr::ch<'A'>, ctr::ch<'B'>, ctr::ch<'|'>, ctr::ch<'C'>, ctr::ch<'|'>>>
  );
}
