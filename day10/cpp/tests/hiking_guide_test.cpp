/**
 * @file hiking_guide_test.cpp
 * @brief Unit tests for the HikeGuide class.
 */
#include "gtest/gtest.h"
// #include "gmock/gmock.h"
#include <sstream>
#include "hiking_guide.hpp"

/**
 * @test GetScore
 * @brief Tests score, the number of unique ending positions in a hiking map.
 */
TEST(HikeGuideTest, GetScore)
{   
    std::vector<std::vector<size_t>> map {{8,9,0,1,0,1,2,3,},{7,8,1,2,1,8,7,4,},{8,7,4,3,0,9,6,5,},{9,6,5,4,9,8,7,4,},{4,5,6,7,8,9,0,3,},{3,2,0,1,9,0,1,2,},{0,1,3,2,9,8,0,1,},{1,0,4,5,6,7,3,2,}};
    HikeGuide hike_guide(map);
    EXPECT_EQ(hike_guide.get_score(), 36);
}

/**
 * @test GetScore
 * @brief Tests sum of rating of trailheads, the number of ways to reach ending positions in a hiking map from a trailhead.
 */
TEST(HikeGuideTest, SumOfRatingOfTrailheads)
{   
    std::vector<std::vector<size_t>> map {{8,9,0,1,0,1,2,3,},{7,8,1,2,1,8,7,4,},{8,7,4,3,0,9,6,5,},{9,6,5,4,9,8,7,4,},{4,5,6,7,8,9,0,3,},{3,2,0,1,9,0,1,2,},{0,1,3,2,9,8,0,1,},{1,0,4,5,6,7,3,2,}};
    HikeGuide hike_guide(map);
    EXPECT_EQ(hike_guide.get_sum_rating_of_all_trailheads(), 81);
}
