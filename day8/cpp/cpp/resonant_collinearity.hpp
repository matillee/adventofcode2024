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

namespace std
{
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
 * @class Frequency
 * @brief Represents a frequency in the grid.
 */
class Frequency
{
public:
    Frequency(char frequency_char, const std::vector<std::vector<char>> &grid);
    char get_frequency_char() const;
    std::unordered_set<Position> get_antinode_positions(const std::vector<std::vector<char>> &grid, bool resonant_harmonics) const;
    bool operator==(const Frequency &other) const;

private:
    char frequency_char;
    std::unordered_set<Position> frequency_positions;
    void find_frequency_positions(const std::vector<std::vector<char>> &grid);
    std::unordered_set<Position> find_antinode_positions_from_frequency_positions(const int &grid_row_size, const int &grid_column_size, bool resonant_harmonics) const;
    void calculate_antinode_positions(std::unordered_set<Position> &antinode_positions, const Position &compare_pos, const Position &pos, const int &grid_row_size, const int &grid_column_size, bool resonant_harmonics) const;
};

namespace std
{
    template <>
    struct hash<Frequency>
    {
        std::size_t operator()(const Frequency &f) const
        {
            return std::hash<char>()(f.get_frequency_char());
        }
    };
}

/**
 * @class ResonantCollinearity
 * @brief Handles the logic for detecting resonant collinearity in a grid.
 */
class ResonantCollinearity
{
public:
    ResonantCollinearity(const std::vector<std::vector<char>> &grid, bool resonant_harmonics = false);
    std::unordered_set<Position> get_unique_antinode_positions();
    size_t get_number_of_unique_antinode_positions();

protected:
    void process_frequencies(const std::vector<std::vector<char>> &grid, bool resonant_harmonics);

private:
    std::unordered_set<Position> unique_antinode_positions;
    std::unordered_set<Frequency> frequencies;
};

/**
 * @class ManagerClass
 * @brief Manages file I/O, setup, and analysis of antenna antinodes.
 */
class ManagerClass
{
public:
    ManagerClass(const std::string &input_file_name);
    std::unordered_set<Position> get_unique_antinode_positions();
    size_t get_number_of_unique_antinode_positions();
    std::unordered_set<Position> get_unique_antinode_positions_with_resonant_harmonics();

private:
    std::vector<std::vector<char>> grid;
    std::vector<std::vector<char>> read_input(const std::string &filename);
};