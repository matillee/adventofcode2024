/**
 * @file garden_groups_test.cpp
 * @brief Unit tests for the Garden Groups classes.
 */
#include "gtest/gtest.h"
// #include "gmock/gmock.h"
#include <sstream>
#include "garden_groups.hpp"

/**
 * @test GetScore
 * @brief Tests fence pricing for A garden.
 */
TEST(GardenGroupsTest, GetAFencePricing)
{
    std::vector<std::vector<char>> garden{{'A', 'A', 'A', 'A'},
                                          {'B', 'B', 'C', 'D'},
                                          {'B', 'B', 'C', 'C'},
                                          {'E', 'E', 'E', 'C'}};
    Gardener gardener(garden);
    EXPECT_EQ(gardener.get_fence_pricing(false), 140);
}

/**
 * @test GetScore
 * @brief Tests fence pricing for X garden
 */
TEST(GardenGroupsTest, GetXFencePricing)
{
    std::vector<std::vector<char>> garden{{'O', 'O', 'O', 'O', 'O'},
                                          {'O', 'X', 'O', 'X', 'O'},
                                          {'O', 'O', 'O', 'O', 'O'},
                                          {'O', 'X', 'O', 'X', 'O'},
                                          {'O', 'O', 'O', 'O', 'O'}};
    Gardener gardener(garden);
    EXPECT_EQ(gardener.get_fence_pricing(false), 772);
}

/**
 * @test GetScore
 * @brief Tests fence pricing for R garden
 */
TEST(GardenGroupsTest, GetRFencePricing)
{
    std::vector<std::vector<char>> garden{{'R', 'R', 'R', 'R', 'I', 'I', 'C', 'C', 'F', 'F'},
                                          {'R', 'R', 'R', 'R', 'I', 'I', 'C', 'C', 'C', 'F'},
                                          {'V', 'V', 'R', 'R', 'R', 'C', 'C', 'F', 'F', 'F'},
                                          {'V', 'V', 'R', 'C', 'C', 'C', 'J', 'F', 'F', 'F'},
                                          {'V', 'V', 'V', 'V', 'C', 'J', 'J', 'C', 'F', 'E'},
                                          {'V', 'V', 'I', 'V', 'C', 'C', 'J', 'J', 'E', 'E'},
                                          {'V', 'V', 'I', 'I', 'I', 'C', 'J', 'J', 'E', 'E'},
                                          {'M', 'I', 'I', 'I', 'I', 'I', 'J', 'J', 'E', 'E'},
                                          {'M', 'I', 'I', 'I', 'S', 'I', 'J', 'E', 'E', 'E'},
                                          {'M', 'M', 'M', 'I', 'S', 'S', 'J', 'E', 'E', 'E'}};
    Gardener gardener(garden);
    EXPECT_EQ(gardener.get_fence_pricing(false), 1930);
}

/**
 * @test GetScore
 * @brief Tests fence pricing for A garden with sides.
 */
TEST(GardenGroupsTest, GetAFencePricingWithSides)
{
    std::vector<std::vector<char>> garden{{'A', 'A', 'A', 'A'},
                                          {'B', 'B', 'C', 'D'},
                                          {'B', 'B', 'C', 'C'},
                                          {'E', 'E', 'E', 'C'}};
    Gardener gardener(garden);
    EXPECT_EQ(gardener.get_fence_pricing(true), 80);
}

/**
 * @test GetScore
 * @brief Tests fence pricing for E garden with sides.
 */
TEST(GardenGroupsTest, GetEFencePricingWithSides)
{
    std::vector<std::vector<char>> garden{{'E', 'E', 'E', 'E', 'E'},
                                          {'E', 'X', 'X', 'X', 'X'},
                                          {'E', 'E', 'E', 'E', 'E'},
                                          {'E', 'X', 'X', 'X', 'X'},
                                          {'E', 'E', 'E', 'E', 'E'}};
    Gardener gardener(garden);
    EXPECT_EQ(gardener.get_fence_pricing(true), 236);
}
/*
     -----
    |EEEEE|
      ----
    |E|XXXX
      ----
    |EEEEE
      ----
    |EXXXX
      ----
    |EEEEE
      ----
 */
/**
 * @test GetScore
 * @brief Tests fence pricing for AB garden with sides.
 */
TEST(GardenGroupsTest, GetABFencePricingWithSides)
{
    std::vector<std::vector<char>> garden{{'A', 'A', 'A', 'A', 'A', 'A'},
                                          {'A', 'A', 'A', 'B', 'B', 'A'},
                                          {'A', 'A', 'A', 'B', 'B', 'A'},
                                          {'A', 'B', 'B', 'A', 'A', 'A'},
                                          {'A', 'B', 'B', 'A', 'A', 'A'},
                                          {'A', 'A', 'A', 'A', 'A', 'A'}};
    Gardener gardener(garden);
    EXPECT_EQ(gardener.get_fence_pricing(true), 368);
}

/**
 * @test GetScore
 * @brief Tests fence pricing for X garden with sides.
 */
TEST(GardenGroupsTest, GetXFencePricingWithSides)
{
    std::vector<std::vector<char>> garden{{'O', 'O', 'O', 'O', 'O'},
                                          {'O', 'X', 'O', 'X', 'O'},
                                          {'O', 'O', 'O', 'O', 'O'},
                                          {'O', 'X', 'O', 'X', 'O'},
                                          {'O', 'O', 'O', 'O', 'O'}};
    Gardener gardener(garden);
    EXPECT_EQ(gardener.get_fence_pricing(true), 436);
}

/**
 * @test GetScore
 * @brief Tests fence pricing for R garden with sides.
 */
TEST(GardenGroupsTest, GetRFencePricingWithSides)
{
    std::vector<std::vector<char>> garden{{'R', 'R', 'R', 'R', 'I', 'I', 'C', 'C', 'F', 'F'},
                                          {'R', 'R', 'R', 'R', 'I', 'I', 'C', 'C', 'C', 'F'},
                                          {'V', 'V', 'R', 'R', 'R', 'C', 'C', 'F', 'F', 'F'},
                                          {'V', 'V', 'R', 'C', 'C', 'C', 'J', 'F', 'F', 'F'},
                                          {'V', 'V', 'V', 'V', 'C', 'J', 'J', 'C', 'F', 'E'},
                                          {'V', 'V', 'I', 'V', 'C', 'C', 'J', 'J', 'E', 'E'},
                                          {'V', 'V', 'I', 'I', 'I', 'C', 'J', 'J', 'E', 'E'},
                                          {'M', 'I', 'I', 'I', 'I', 'I', 'J', 'J', 'E', 'E'},
                                          {'M', 'I', 'I', 'I', 'S', 'I', 'J', 'E', 'E', 'E'},
                                          {'M', 'M', 'M', 'I', 'S', 'S', 'J', 'E', 'E', 'E'}};
    Gardener gardener(garden);
    EXPECT_EQ(gardener.get_fence_pricing(true), 1206);
}
