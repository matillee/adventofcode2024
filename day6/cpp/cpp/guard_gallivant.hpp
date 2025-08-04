#pragma once
#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <unordered_set>
#include <map>
#include <algorithm>

/**
 * @class GuardMovement
 * @brief Represents the guard's position and direction.
 */
class GuardMovement
{
public:
    char direction;
    int x_position;
    int y_position;
    GuardMovement(const int &x, const int &y, const char &dir);
    bool operator==(const GuardMovement &other) const;
    friend std::ostream &operator<<(std::ostream &os, const GuardMovement &gm);
};

/**
 * @class Position
 * @brief Represents a coordinate on the map.
 */
class Position
{
public:
    int x_position;
    int y_position;
    Position(const int &x, const int &y);
    bool operator==(const Position &other) const;
};

/**
 * @namespace std
 * @brief Hash specializations for GuardMovement and Position for use in unordered_set.
 */
namespace std
{
    template <>
    struct hash<GuardMovement>
    {
        std::size_t operator()(const GuardMovement &gm) const
        {
            return std::hash<char>()(gm.direction) ^ std::hash<int>()(gm.x_position) ^ std::hash<int>()(gm.y_position);
        }
    };

    template <>
    struct hash<Position>
    {
        std::size_t operator()(const Position &p) const
        {
            return std::hash<int>()(p.x_position) ^ std::hash<int>()(p.y_position);
        }
    };
}

/**
 * @class OutOfBoundsException
 * @brief Exception thrown when the guard moves out of bounds.
 */
class OutOfBoundsException : public std::exception
{
public:
    char *what();
};

/**
 * @class GuardBehaviour
 * @brief Handles guard movement logic and obstacle encounters.
 */
class GuardBehaviour
{
public:
    static GuardMovement patrol_area(const GuardMovement &current, const std::vector<std::vector<char>> &map);

private:
    static const std::map<char, std::pair<int, int>> &direction_map();
    static char handle_obstacle_encounter(const char &direction);
    static bool guard_is_out_of_bounds(const int &new_x, const int &new_y, const std::vector<std::vector<char>> &map);
};

/**
 * @class GuardSimulation
 * @brief Simulates the guard's patrol and tracks visited positions.
 */
class GuardSimulation
{
public:
    GuardSimulation(std::vector<std::vector<char>> starting_map, GuardMovement initial_guard_movement);
    std::unordered_set<Position> get_patrolled_area();
    bool results_in_loop();

private:
    std::vector<std::vector<char>> map;
    std::unordered_set<GuardMovement> visited_positions_with_direction;
    std::unordered_set<Position> visited_positions;
    GuardMovement initial_guard_movement;
    bool guard_patrols_area_and_leaves_map();
    void print_patrolled_area(GuardMovement current_guard_movement);
};

/**
 * @class ManagerClass
 * @brief Manages file I/O, simulation setup, and analysis of obstructions.
 */
class ManagerClass
{
public:
    ManagerClass(const std::string &input_file_name);
    std::unordered_set<Position> get_all_possible_obstructions_to_create_guard_loops();
    size_t get_number_of_obstructions_for_guard_loops();
    std::unordered_set<Position> get_patrolled_area();
    size_t get_number_of_patrolled_positions();

private:
    std::vector<std::vector<char>> starting_map;
    GuardMovement initial_guard_movement;
    std::vector<std::vector<char>> read_input(const std::string &filename);
    GuardMovement find_guard_in_map(std::vector<std::vector<char>> map);
};