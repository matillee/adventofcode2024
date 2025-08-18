/**
 * @file plutonian_pebbles.hpp
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

// When multiple garden plots are growing the same type of plant and are touching (horizontally or vertically), they form a region.
class Region
{
public:
    // the price of fence required for a region is found by multiplying that region's area by its perimeter.
    size_t get_fence_pricing();

private:
    size_t area;
    size_t perimenter;
    // The perimeter of a region is the number of sides of garden plots in the region that do not touch another garden plot in the same region.
};

class GardenGroup
{
public:
    GardenGroup(char plant, const std::vector<std::vector<char>> &garden);
    char plant;
    // The total price of fencing all regions on a map is found by adding together the price of fence for every region on the map.
    size_t get_fence_pricing();

private:
    std::vector<std::vector<bool>> visited;
    std::vector<Region> regions;
    
    // Gör detta innan garden groups - lägg till gardengropups baserat på plant type sen.
    std::vector<Region> find_plant_regions(const std::vector<std::vector<char>> &garden);
    std::optional<std::pair<size_t, size_t>> find_plant(const std::vector<std::vector<char>> &garden);
    Region get_plant_region(const std::pair<size_t, size_t> &plant_coordinates, const std::vector<std::vector<char>> &garden);
};

class Gardener
{
public:
    Gardener(std::vector<std::vector<char>> garden);
    size_t get_fence_pricing();

private:
    std::vector<GardenGroup> garden_groups;
    std::vector<GardenGroup> get_garden_groups(const std::vector<std::vector<char>> &garden);
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
    size_t get_fence_pricing();

private:
    std::vector<std::vector<char>> garden;
    Gardener gardener;
    std::vector<std::vector<char>> read_input(const std::string &filename);
};