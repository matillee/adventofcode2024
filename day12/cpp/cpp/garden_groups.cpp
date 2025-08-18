#include "garden_groups.hpp"

Gardener::Gardener(std::vector<std::vector<char>> garden) : garden_groups(get_garden_groups(garden)) {};

std::vector<GardenGroup> Gardener::get_garden_groups(const std::vector<std::vector<char>> &garden)
{
    std::vector<GardenGroup> groups;
    std::unordered_set<char> plant_types;
    for (const auto &garden_row : garden)
    {
        for (const char plant : garden_row)
        {
            if (!plant_types.contains(plant))
            {
                plant_types.emplace(plant);
                groups.emplace_back(GardenGroup(plant, garden));
            }
        }
    }
    return groups;
}

size_t Gardener::get_fence_pricing()
{
    size_t sum = 0;
    for (auto &garden_group : garden_groups)
    {
        sum += garden_group.get_fence_pricing();
    }
    return sum;
}

GardenGroup::GardenGroup(char plant, const std::vector<std::vector<char>> &garden)
    : plant(plant),
      visited({garden.size(), std::vector<bool>(garden[0].size(), false)}),
      regions(find_plant_regions(garden)) {};

size_t GardenGroup::get_fence_pricing()
{
    size_t sum = 0;
    for (auto &region : regions)
    {
        sum += region.get_fence_pricing();
    }
    return sum;
}

std::vector<Region> GardenGroup::find_plant_regions(const std::vector<std::vector<char>> &garden)
{
    // Medan vi inte har besökt alla plastser i garden... Kom på annat villkor. while true och sen break om vi inte hittar ny plant_coordinates?
    // size_t visits = 0;
    // while (visits < garden.size() * garden[0].size())
    while (true)
    {
        auto plant_coordinates = find_plant(garden);
        if (!plant_coordinates)
            break;
        regions.emplace_back(get_plant_region(plant_coordinates.value(), garden));
    }
};

std::optional<std::pair<size_t, size_t>> GardenGroup::find_plant(const std::vector<std::vector<char>> &garden)
{
    std::pair<size_t, size_t> position;
    for (auto &v : visited)
    {
        auto found = std::find_if(v.begin(), v.end(), false);
        if (found != v.end())
        {
            position.first = &v - &visited[0];   // row
            position.second = found - v.begin(); // col
            return position;
        }
    }
    return std::nullopt;
}

Region GardenGroup::get_plant_region(const std::pair<size_t, size_t> &plant_coordinates, const std::vector<std::vector<char>> &garden)
{
}