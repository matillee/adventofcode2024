#include "gtest/gtest.h"
// #include "gmock/gmock.h"
#include <sstream>
#include "guard_gallivant.hpp"

TEST(PositionTest, EqualityOperator)
{
    Position a(1, 2), b(1, 2), c(2, 1);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
}

TEST(GuardMovementTest, EqualityAndStream)
{
    GuardMovement a(1, 2, '^'), b(1, 2, '^'), c(1, 2, '>');
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
    std::stringstream ss;
    ss << a;
    EXPECT_NE(ss.str().find("GuardMovement(x=1, y=2, dir=^)"), std::string::npos);
}

TEST(GuardBehaviourTest, PatrolAreaMovesCorrectly)
{
    std::vector<std::vector<char>> map = {
        {'.', '.', '.'},
        {'.', '^', '.'},
        {'.', '.', '.'}};
    GuardMovement start(1, 1, '^');
    GuardMovement next = GuardBehaviour::patrol_area(start, map);
    EXPECT_EQ(next.x_position, 1);
    EXPECT_EQ(next.y_position, 0);
    EXPECT_EQ(next.direction, '^');
}

TEST(GuardBehaviourTest, PatrolAreaTurnsOnObstacle)
{
    std::vector<std::vector<char>> map = {
        {'.', '#', '.'},
        {'.', '^', '.'},
        {'.', '.', '.'}};
    GuardMovement start(1, 1, '^');
    GuardMovement next = GuardBehaviour::patrol_area(start, map);
    EXPECT_EQ(next.x_position, 1);
    EXPECT_EQ(next.y_position, 1);
    EXPECT_EQ(next.direction, '>');
}

TEST(GuardSimulationTest, DetectsLoop)
{
    std::vector<std::vector<char>> map_with_loop = {
        {'.', '.', '.', '.', '#', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '#', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '#', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '#', '.', '#', '^', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.', '#', '.'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '#', '.', '.', '.'}};
    GuardMovement start(4, 6, '^');
    GuardSimulation sim(map_with_loop, start);
    EXPECT_TRUE(sim.results_in_loop());
}

TEST(GuardSimulationTest, DetectsExit)
{
    std::vector<std::vector<char>> map = {
        {'.', '.', '.'},
        {'.', '^', '.'},
        {'.', '.', '.'}};
    GuardMovement start(1, 1, '^');
    GuardSimulation sim(map, start);
    EXPECT_FALSE(sim.results_in_loop());
}

TEST(ManagerClassTest, ThrowsIfFileDoesNotExist)
{
    EXPECT_THROW(ManagerClass("nonexistent_file.txt"), std::runtime_error);
}

TEST(ManagerClassTest, ThrowsIfNoGuardInMap)
{
    EXPECT_THROW(ManagerClass mgr("../no_guard"), std::invalid_argument);
}

TEST(ManagerClassTest, PatrolledAreaCount)
{
    std::vector<std::vector<char>> map = {
        {'.', '.', '.'},
        {'.', '^', '.'},
        {'.', '.', '.'}};
    GuardMovement start(1, 1, '^');
    GuardSimulation sim(map, start);
    EXPECT_EQ(sim.get_patrolled_area().size(), 2); // Should visit 2 unique positions
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}