#include "gtest/gtest.h"
// #include "gmock/gmock.h"
#include <sstream>
#include "resonant_collinearity.hpp"

TEST(PositionTest, EqualityOperator)
{
    Position a(1, 2), b(1, 2), c(2, 1);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
}

TEST(FrequencyTest, EqualityOperator)
{
    Frequency f1('A', {{'.', '.', '.'}, {'.', 'A', '.'}, {'.', '.', '.'}});
    Frequency f2('A', {{'.', '.', '.'}, {'.', 'A', '.'}, {'.', '.', '.'}});
    Frequency f3('B', {{'.', '.', '.'}, {'.', 'B', '.'}, {'.', '.', '.'}});

    EXPECT_TRUE(f1 == f2);
    EXPECT_FALSE(f1 == f3);
}

TEST(ResonantCollinearityTest, UniqueAntinodePositions)
{
    std::vector<std::vector<char>> grid = {
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '0', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '0', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '0', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '0', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', 'A', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', 'A', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', 'A', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
    };

    ResonantCollinearity resonant_collinearity(grid, false);
    auto antinode_positions = resonant_collinearity.get_unique_antinode_positions();

    EXPECT_EQ(antinode_positions.size(), 14);
}


TEST(ResonantCollinearityTest, UniqueAntinodePositionsWithResonantHarmonics)
{
    std::vector<std::vector<char>> grid = {
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '0', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '0', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '0', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '0', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', 'A', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', 'A', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', 'A', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
    };

    ResonantCollinearity resonant_collinearity(grid, true);
    auto antinode_positions = resonant_collinearity.get_unique_antinode_positions();

    EXPECT_EQ(antinode_positions.size(), 34);
}