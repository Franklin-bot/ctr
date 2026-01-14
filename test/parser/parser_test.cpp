#include "../../src/common/fixed_string.hpp"
#include "../../src/common/pure/algos.hpp"

#include <gtest/gtest.h>

#include <type_traits>

TEST(ShuntingYardTest, Atoms)
{
  static_assert(
    std::is_same_v<ctr::ShuntingYard<ctr::fixed_string{"A"}>, ctr::deque<ctr::ch<'A'>>>
  );

  static_assert(std::is_same_v<
                ctr::ShuntingYard<ctr::fixed_string{"A|B"}>,
                ctr::deque<ctr::ch<'A'>, ctr::ch<'B'>, ctr::ch<'|'>>>);
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
