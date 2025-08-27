/**
 * @file garden_groups.hpp
 * @brief Declares classes and methods for Garden Group analysis
 */
#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <variant>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <optional>

/**
 * @class Position
 * @brief Represents a coordinate on the map.
 * @param x_position The x (#columns) position.
 * @param y_position The y (#rows) position.
 */
class Position
{
public:
    int x_position, y_position;
    Position(int x, int y);
    bool operator==(const Position &other) const;
    friend std::ostream &operator<<(std::ostream &os, const Position &rs);
};

/**
 * @enum SideStatus
 * @brief Represents the status of a region side.
 * Possible values: ADJACENT_TO_OTHER_PLANT_TYPE, OUT_OF_BOUNDS, AVAILABLE, VISITED.
 */
enum class SideStatus
{
    ADJACENT_TO_OTHER_PLANT_TYPE,
    OUT_OF_BOUNDS,
    AVAILABLE,
    VISITED
};

/**
 * @enum SideOrientation
 * @brief Represents the orientation of a region side.
 * Possible values: UPPER, LOWER, LEFT, RIGHT.
 */
enum SideOrientation
{
    UPPER,
    LOWER,
    LEFT,
    RIGHT,
};

/**
 * @class TraversePosition
 * @brief Represents a position used during traversal of the garden grid.
 *
 * Stores coordinates and state information for traversal algorithms.
 */
class TraversePosition : public Position
{
public:
    /**
     * @brief Constructs a TraversePosition object.
     * @param x The x-coordinate.
     * @param y The y-coordinate.
     * @param value The value at the position.
     */
    TraversePosition(int x, int y, char value) : Position(x, y), value(value) {};
    std::unordered_map<SideOrientation, SideStatus> side_status_map;
    size_t get_number_of_perimeter_sides();
    void update_side_status(SideOrientation side_orientation, SideStatus status);
    char value;
    friend std::ostream &operator<<(std::ostream &os, const TraversePosition &p);
};

/**
 * @class RegionSide
 * @brief Represents a side of a region in the garden grid.
 *
 * Stores orientation, status, and position information for a region side.
 */
class RegionSide
{
public:
    RegionSide(SideOrientation orientation, Position start_position, std::vector<TraversePosition> search_positions);
    SideOrientation orientation;
    Position start_position, end_position;
    bool operator==(const RegionSide &other) const;
    friend std::ostream &operator<<(std::ostream &os, const RegionSide &rs);

private:
    Position get_end_position(std::vector<TraversePosition> search_positions);
};

/**
 * @namespace std
 * @brief Hash specializations for Position and RegionSide for use in unordered_set.
 */
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

    template <>
    struct hash<RegionSide>
    {
        std::size_t operator()(const RegionSide &rs) const
        {
            return std::hash<Position>()(rs.start_position) ^ std::hash<Position>()(rs.end_position) ^ std::hash<SideOrientation>()(rs.orientation);
        }
    };
}

/**
 * @class Region
 * @brief Represents a contiguous (horizontal/vertical) region in the garden grid.
 *
 * Stores the plant type, positions, and sides that define the region.
 */
class Region
{
public:
    Region(char plant, size_t r, size_t c, const std::vector<std::vector<char>> &garden);
    size_t get_fence_pricing(bool with_sides);
    char plant;
    std::vector<TraversePosition> traverse_positions;
    bool operator==(const Region &other) const;
    friend std::ostream &operator<<(std::ostream &os, const Region &r);

private:
    std::vector<std::vector<bool>> visited_garden_plots;
    const std::vector<std::vector<char>> &garden;
    void get_region_plots(size_t r, size_t c, const std::vector<std::vector<char>> &garden);
    size_t get_area();
    size_t get_perimeter();
    size_t get_num_of_region_sides();
    std::optional<TraversePosition> get_traverse_position(int r, int c, const std::vector<std::vector<char>> &garden);
    void traverse_from_position(TraversePosition &tp, const std::vector<std::vector<char>> &garden);
    void update_side_status(TraversePosition &tp, const std::vector<std::vector<char>> &garden);
};

/**
 * @class GardenGroup
 * @brief Represents a group of plants of the same type in the garden grid.
 *
 * Stores the plant type and the positions of all plants of that type in the garden.
 */
class GardenGroup
{
public:
    GardenGroup(char plant);
    char plant;
    size_t get_fence_pricing(bool with_sides);
    void add_region(Region region);
    bool operator==(const GardenGroup &other) const;

private:
    std::vector<Region> regions;
};

/**
 * @namespace std
 * @brief Hash specializations for GardenGroup for use in unordered_set.
 */
namespace std
{
    template <>
    struct hash<GardenGroup>
    {
        std::size_t operator()(const GardenGroup &gg) const
        {
            return std::hash<int>()(gg.plant) ^ std::hash<char>()('s') ^ std::hash<char>()('a') ^ std::hash<char>()('l') ^ std::hash<char>()('t');
        }
    };

    template <>
    struct hash<Region>
    {
        std::size_t operator()(const Region &r) const
        {
            std::string h_string;
            h_string += std::hash<char>()(r.plant);
            for (auto &plot : r.traverse_positions)
            {
                h_string += std::to_string(plot.x_position) + std::to_string(plot.y_position);
            }
            return std::hash<std::string>()(h_string);
        }
    };
}

/**
 * @class Gardener
 * @brief Responsible for analyzing the garden grid and finding groups of plants.
 *
 * Provides methods to identify and process contiguous groups of plants in the garden.
 */
class Gardener
{
public:
    Gardener(std::vector<std::vector<char>> garden);
    size_t get_fence_pricing(bool with_sides);

private:
    std::vector<std::vector<bool>> visited_garden_plots;
    std::unordered_map<char, GardenGroup> garden_groups;
    std::unordered_map<char, GardenGroup> find_garden_groups(const std::vector<std::vector<char>> &garden);
    Region get_plant_region(size_t r, size_t c, const std::vector<std::vector<char>> &garden);
};

/**
 * @class ManagerClass
 * @brief Handles reading the garden from file and providing the fence pricing interface.
 * @param input_file_name The path to the input file.
 */
class ManagerClass
{
public:
    ManagerClass(const std::string &input_file_name);
    size_t get_fence_pricing(bool with_sides);

private:
    std::vector<std::vector<char>> garden;
    Gardener gardener;
    std::vector<std::vector<char>> read_input(const std::string &filename);
};