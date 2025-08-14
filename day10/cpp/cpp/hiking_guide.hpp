/**
 * @file hiking_guide.hpp
 * @brief Declares classes and methods for hiking map analysis, trailhead scoring, and DFS traversal.
 */
#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <variant>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>

/**
 * @class Position
 * @brief Represents a coordinate on the map.
 * @param x_position The x (#columns) position.
 * @param y_position The y (#rows) position.
 * @param value The content of the position.
 */
class Position
{
public:
    int x_position, y_position;
    size_t value;
    Position(int x, int y, size_t value);
    bool operator==(const Position &other) const;
};

/**
 * @namespace std
 * @brief Hash specializations for Position for use in unordered_set.
 */
namespace std
{
    template <>
    struct hash<Position>
    {
        std::size_t operator()(const Position &p) const
        {
            return std::hash<int>()(p.x_position) ^ std::hash<int>()(p.y_position) ^ std::hash<size_t>()(p.value);
        }
    };
}

/**
 * @class HikeTrailsDFS
 * @brief Performs DFS traversal on the hiking map to find reachable ending positions and ratings.
 * @param grid The hiking map as a 2D vector.
 * @param stop_value The target value to stop DFS.
 * @param height_increment The increment for each step in height.
 */
class HikeTrailsDFS
{
public:
    HikeTrailsDFS(const std::vector<std::vector<size_t>> &grid, size_t stop_value, size_t height_increment);
    std::unordered_set<Position> get_reachable_ending_positions(Position trail_head);
    std::unordered_map<Position, size_t> get_rating_of_trailheads(Position trail_head);

private:
    const std::vector<std::vector<size_t>> &grid;
    size_t rows, cols, stop_value, height_increment;
    bool processed_dfs;
    std::vector<std::vector<bool>> visited;
    std::unordered_set<Position> reachable_ending_positions;
    std::unordered_map<Position, size_t> rating_of_trailheads;
    void dfs(size_t r, size_t c, size_t target_value);
};

/**
 * @class Trailhead
 * @brief Represents a trailhead on the hiking map and provides scoring and rating methods.
 * @param map The hiking map as a 2D vector.
 * @param starting_pos The starting position for the trailhead.
 * @param ending_height The target ending height for the trail.
 * @param height_increment The increment for each step in height.
 */
class Trailhead
{
public:
    Trailhead(const std::vector<std::vector<size_t>> &map, const Position &starting_pos, size_t ending_height, size_t height_increment);
    size_t get_score();
    size_t get_rating_of_trailheads();

private:
    const std::vector<std::vector<size_t>> &map;
    const Position &starting_pos;
    size_t ending_height, height_increment;
    HikeTrailsDFS hike_trails_dfs;
    std::unordered_set<Position> reachable_ending_positions;
};

/**
 * @class HikeGuide
 * @brief Manages all trailheads and provides aggregate scoring and rating methods.
 * @param map The hiking map as a 2D vector.
 */
class HikeGuide
{
public:
    HikeGuide(const std::vector<std::vector<size_t>> &map);
    size_t get_score();
    size_t get_sum_rating_of_all_trailheads();

private:
    const std::vector<std::vector<size_t>> &map;
    std::vector<Trailhead> trail_heads;
    void find_trail_heads();
};

/**
 * @class ManagerClass
 * @brief Handles reading the map from file and providing the score interface.
 * @param input_file_name The path to the input file.
 * @param map The map as a 2D vector of size_t integers.
 */
class ManagerClass
{
public:
    ManagerClass(const std::string &input_file_name);
    size_t get_score();
    size_t get_sum_rating_of_all_trailheads();

private:
    std::vector<std::vector<size_t>> map;
    HikeGuide hike_guide;
    std::vector<std::vector<size_t>> read_input(const std::string &filename);
    size_t convert_char_to_size_t(char c);
};