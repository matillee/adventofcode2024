/**
 * @file hiking_guide_test.cpp
 * @brief Unit tests for the HikeGuide class.
 */
#include "gtest/gtest.h"
// #include "gmock/gmock.h"
#include <sstream>
#include "plutonian_pebbles.hpp"

/**
 * @test GetScore
 * @brief Tests score, the number of pebbles after blinking 25 times
 */
TEST(PlutonianPebbleTransformerTest, GetNumberOfPebbles)
{   
    std::vector<size_t> starting_pebble_order {125, 17};
    PlutonianPebbleTransformer pebble_transformer(starting_pebble_order);
    EXPECT_EQ(pebble_transformer.get_number_of_pebbles_after_blinking(25), 55312);
}
