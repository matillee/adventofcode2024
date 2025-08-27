#include "garden_groups.hpp"

/**
 * @brief Constructs a Position object.
 * @param x The x (#columns) position.
 * @param y The y (#rows) position.
 */
Position::Position(int x, int y)
    : x_position(x), y_position(y) {};

/**
 * @brief Equality operator for Position.
 * @param other The other Position to compare.
 * @return True if positions are equal.
 */
bool Position::operator==(const Position &other) const
{
    return x_position == other.x_position && y_position == other.y_position;
};

/**
 * @brief Output stream operator for Position.
 * @param os The output stream.
 * @param p The Position to print.
 * @return The output stream.
 */
std::ostream &operator<<(std::ostream &os, const Position &p)
{
    os << "Position(x=" << p.x_position << ", y=" << p.y_position << ")";

    return os;
}

/**
 * @brief Updates the status of a side for this position.
 * @param side_orientation The orientation of the side.
 * @param status The new status to set.
 */
void TraversePosition::update_side_status(SideOrientation side_orientation, SideStatus status)
{
    side_status_map[side_orientation] = status;
}

/**
 * @brief Gets the number of perimeter sides for this position.
 * @return The number of perimeter sides.
 */
size_t TraversePosition::get_number_of_perimeter_sides()
{
    size_t sides = 0;
    for (const auto &pair : side_status_map)
    {
        if (pair.second == SideStatus::ADJACENT_TO_OTHER_PLANT_TYPE || pair.second == SideStatus::OUT_OF_BOUNDS)
        {
            sides++;
        }
    }
    return sides;
}

/**
 * @brief Output stream operator for TraversePosition.
 * @param os The output stream.
 * @param p The TraversePosition to print.
 * @return The output stream.
 */
std::ostream &operator<<(std::ostream &os, const TraversePosition &p)
{
    os << "TraversePosition(x=" << p.x_position << ", y=" << p.y_position << ", value=" << p.value << ", status=[";

    for (auto side : p.side_status_map)
    {
        std::string tmp_status;
        switch (side.second)
        {
        case SideStatus::ADJACENT_TO_OTHER_PLANT_TYPE:
            tmp_status = "ADJACENT_TO_OTHER_PLANT_TYPE";
            break;
        case SideStatus::OUT_OF_BOUNDS:
            tmp_status = "OUT_OF_BOUNDS";
            break;
        case SideStatus::AVAILABLE:
            tmp_status = "AVAILABLE";
            break;
        case SideStatus::VISITED:
            tmp_status = "VISITED";
            break;
        }

        std::string tmp_orientation;
        switch (side.first)
        {
        case SideOrientation::UPPER:
            tmp_orientation = "UPPER";
            break;
        case SideOrientation::LOWER:
            tmp_orientation = "LOWER";
            break;
        case SideOrientation::LEFT:
            tmp_orientation = "LEFT";
            break;
        case SideOrientation::RIGHT:
            tmp_orientation = "RIGHT";
            break;
        default:
            break;
        }
        os << "{o=" << tmp_orientation << ", s=" << tmp_status << "}, ";
    }

    os << "])" << std::endl;
    return os;
}

/**
 * @brief Equality operator for RegionSide.
 * @param other The other RegionSide to compare.
 * @return True if regionsides are equal.
 */
bool RegionSide::operator==(const RegionSide &other) const
{
    if (orientation != other.orientation)
        return false;

    // Check if other RegionSide lies between this start_position and end_position (inclusive)

    if (other.start_position == start_position && other.end_position == end_position)
    {
        // Trivially equal
        return true;
    }

    if (orientation == SideOrientation::LEFT || orientation == SideOrientation::RIGHT)
    {
        if (other.start_position.x_position != start_position.x_position)
        {
            // std::cout << "Other not on same x-axis." << std::endl;
            return false;
        }

        if (other.start_position.y_position < start_position.y_position && other.end_position.y_position < start_position.y_position)
        {
            // std::cout << "Other entirely below this." << std::endl;
            return false;
        }
        else if (other.start_position.y_position > end_position.y_position && other.end_position.y_position > end_position.y_position)
        {
            // std::cout << "Other entirely above this." << std::endl;
            return false;
        }
    }
    else if (orientation == SideOrientation::UPPER || orientation == SideOrientation::LOWER)
    {
        if (other.start_position.y_position != start_position.y_position)
        {
            // std::cout << "Other not on same y-axis." << std::endl;
            return false;
        }

        if (other.start_position.x_position < start_position.x_position && other.end_position.x_position < start_position.x_position)
        {
            // std::cout << "Other entirely left of this." << std::endl;
            return false;
        }
        else if (other.start_position.x_position > end_position.x_position && other.end_position.x_position > end_position.x_position)
        {
            // std::cout << "Other entirely right of this." << std::endl;
            return false;
        }
    }

    // std::cout << "Overlapping sides" << std::endl;
    return true;
};

/**
 * @brief Output stream operator for RegionSide.
 * @param os The output stream.
 * @param rs The RegionSide to print.
 * @return The output stream.
 */
std::ostream &operator<<(std::ostream &os, const RegionSide &rs)
{
    os << "RegionSide(start=" << rs.start_position << ", end=" << rs.end_position << ", o=" << rs.orientation << ")";

    return os;
}

/**
 * @brief Constructs a RegionSide object.
 * @param orientation The orientation of the side.
 * @param start_position The starting position of the side.
 * @param search_positions The positions to search along the side.
 */
RegionSide::RegionSide(SideOrientation orientation, Position start_position, std::vector<TraversePosition> search_positions)
    : orientation(orientation), start_position(start_position), end_position(get_end_position(search_positions)) {};

Position RegionSide::get_end_position(std::vector<TraversePosition> search_positions)
{
    // std::cout << "End position for side starting at " << start_position << " with orientation " << orientation << " is (";
    if (orientation == SideOrientation::LEFT || orientation == SideOrientation::RIGHT)
    {
        // Remove any positions not on the same y_position as start_position
        search_positions.erase(
            std::remove_if(
                search_positions.begin(),
                search_positions.end(),
                [this](const TraversePosition &tp)
                {
                    return tp.x_position != start_position.x_position;
                }),
            search_positions.end());

        // Sort the array on x_position value
        std::sort(
            search_positions.begin(),
            search_positions.end(),
            [](const TraversePosition &a, const TraversePosition &b)
            {
                return a.y_position < b.y_position;
            });

        int max_y = start_position.y_position;
        for (const auto &tp : search_positions)
        {
            if (tp.y_position == max_y + 1 &&
                (tp.side_status_map.at(orientation) == SideStatus::OUT_OF_BOUNDS ||
                 tp.side_status_map.at(orientation) == SideStatus::ADJACENT_TO_OTHER_PLANT_TYPE))
            {
                max_y = tp.y_position;
            }
        }
        // std::cout << start_position.x_position << ", " << max_y << ")" << std::endl;
        return Position(start_position.x_position, max_y);
    }
    else // if (orientation == SideOrientation::UPPER || orientation == SideOrientation::LOWER)
    {
        // Remove any positions not on the same x_position as start_position
        search_positions.erase(
            std::remove_if(
                search_positions.begin(),
                search_positions.end(),
                [this](const TraversePosition &tp)
                {
                    return tp.y_position != start_position.y_position;
                }),
            search_positions.end());

        // Sort the array on y_position value
        std::sort(
            search_positions.begin(),
            search_positions.end(),
            [](const TraversePosition &a, const TraversePosition &b)
            {
                return a.x_position < b.x_position;
            });

        int max_x = start_position.x_position;
        for (const auto &tp : search_positions)
        {
            if (tp.x_position == max_x + 1 &&
                (tp.side_status_map.at(orientation) == SideStatus::OUT_OF_BOUNDS ||
                 tp.side_status_map.at(orientation) == SideStatus::ADJACENT_TO_OTHER_PLANT_TYPE))
            {
                max_x = tp.x_position;
            }
        }
        // std::cout << max_x << ", " << start_position.y_position << ")" << std::endl;
        return Position(max_x, start_position.y_position);
    }
}

/**
 * @brief Constructs a Region object.
 * @param plant The plant type for the region.
 * @param r The row index of the starting position.
 * @param c The column index of the starting position.
 * @param garden The garden grid.
 */
Region::Region(char plant,
               size_t r,
               size_t c,
               const std::vector<std::vector<char>> &garden)
    : plant(plant), garden(garden)
{
    get_region_plots(r, c, garden);
};

/**
 * @brief Output stream operator for Region.
 * @param os The output stream.
 * @param r The Region to print.
 * @return The output stream.
 */
std::ostream &operator<<(std::ostream &os, const Region &r)
{
    os << "Region plots for plant " << r.plant << ": " << std::endl;
    for (int i = 0; i < (int)r.visited_garden_plots.size(); ++i)
    {
        for (int j = 0; j < (int)r.visited_garden_plots[i].size(); ++j)
        {
            os << r.visited_garden_plots[i][j] << " ";
        }
        os << std::endl;
    }
    return os;
}

/**
 * @brief Calculates the fence pricing for the region.
 * @param with_sides Whether to include sides instead of perimeter in the pricing calculation.
 * @return The total fence price.
 */
size_t Region::get_fence_pricing(bool with_sides)
{
    if (traverse_positions.empty())
    {
        return 0;
    }

    std::cout << "Calculating fence pricing for plant " << plant << " region = ";
    if (with_sides)
    {
        // print_region_plots();
        //  the price of fence required for a region is found by multiplying that region's area by the number of sides.
        return get_area() * get_num_of_region_sides();
    }

    // the price of fence required for a region is found by multiplying that region's area by its perimeter.
    return get_area() * get_perimeter();
}

/**
 * @brief Calculates the total area of the region.
 * @return The total area as a size_t value.
 */
size_t Region::get_area()
{
    std::cout << "Area(" << traverse_positions.size() << ") * ";
    return traverse_positions.size();
}

/**
 * @brief Calculates the total perimeter of the region.
 *
 * Iterates through all positions in the region and sums up the number of perimeter sides
 * for each position to compute the total perimeter.
 *
 * @return The total perimeter as a size_t value.
 */
size_t Region::get_perimeter()
{
    size_t perimeter = 0;
    for (auto &position : traverse_positions)
    {
        perimeter += position.get_number_of_perimeter_sides();
    }
    std::cout << "Perimeter(" << perimeter << ")" << std::endl;
    return perimeter;
}

/**
 * @brief Calculates the number of unique sides of the region that are either adjacent to another plant type or out of bounds.
 *
 * Iterates through all positions in the region and checks each side's status.
 * If a side is adjacent to another plant type or out of bounds, it is considered a region side.
 * Ensures that each region side is counted only once, even if encountered multiple times.
 *
 * @return The number of unique region sides that are adjacent to another plant type or out of bounds.
 */
size_t Region::get_num_of_region_sides()
{
    if (traverse_positions.empty())
    {
        return {};
    }
    std::vector<RegionSide> region_sides;
    for (auto &position : traverse_positions)
    {
        for (auto status : position.side_status_map)
        {
            if (status.second == SideStatus::ADJACENT_TO_OTHER_PLANT_TYPE || status.second == SideStatus::OUT_OF_BOUNDS)
            {
                auto rs = RegionSide(status.first, Position(position.x_position, position.y_position), traverse_positions);
                bool exists = false;
                for (const auto &existing_rs : region_sides)
                {
                    if (rs == existing_rs)
                    {
                        exists = true;
                        break;
                    }
                }
                if (!exists)
                {
                    region_sides.emplace_back(rs);
                }
            }
        }
    }
    std::cout << "RegionSides(" << region_sides.size() << ")" << std::endl;
    return region_sides.size();
}

/**
 * @brief Identifies and processes the region plots starting from a given position in the garden.
 *
 * @param r The row index of the starting plot.
 * @param c The column index of the starting plot.
 * @param garden A 2D vector representing the garden layout, where each cell contains a character.
 */
void Region::get_region_plots(size_t r, size_t c, const std::vector<std::vector<char>> &garden)
{
    if (visited_garden_plots.empty() || visited_garden_plots.at(0).size() != garden[0].size())
        visited_garden_plots.resize(garden.size(), std::vector<bool>(garden[0].size(), false));

    if (visited_garden_plots[r][c])
    {
        // std::cout << garden[r][c] << ": Already visited at (" << r << ", " << c << ")." << std::endl;
        return; // Already visited
    }

    auto result = get_traverse_position(r, c, garden);
    if (result.has_value())
    {
        auto tp = result.value();
        traverse_positions.push_back(tp);
        // std::cout << "Traversing from position (" << tp.x_position << ", " << tp.y_position << ")" << std::endl;
        traverse_from_position(tp, garden);
    }
    else
    {
        // std::cout << "No valid TraversePosition at (" << r << ", " << c << "), cannot get region plots." << std::endl;
        return; // No valid TraversePosition
    }
}

/**
 * @brief Retrieves a TraversePosition object for a given row and column in the garden.
 *
 * @param r Row index in the garden grid.
 * @param c Column index in the garden grid.
 * @param garden 2D vector representing the garden layout.
 * @return std::optional<TraversePosition> The TraversePosition at (r, c) if valid, otherwise std::nullopt.
 */
std::optional<TraversePosition> Region::get_traverse_position(int r, int c, const std::vector<std::vector<char>> &garden)
{
    if (r < 0 || r >= static_cast<int>(garden.size()) || c < 0 || c >= static_cast<int>(garden[0].size()))
    {
        std::cout << "Position (" << r << ", " << c << ") is out of garden bounds." << std::endl;
        return std::nullopt;
    }
    TraversePosition tp(c, r, garden[r][c]);
    update_side_status(tp, garden);
    // std::cout << "Created TraversePosition at (" << tp.x_position << ", " << tp.y_position << ")" << std::endl;
    return tp;
}

/**
 * @brief Recursively traverses the garden grid from a given position, marking visited plots and exploring available sides.
 *
 * This method marks the current garden plot as visited and iterates over all sides of the current position.
 * For each side that is available, it calculates the new position in the corresponding direction.
 * If the new position is valid, contains the same plant as the region, and has not been visited,
 * it marks the new plot as visited, updates the side status, and recursively continues traversal from the new position.
 *
 * @param tp The current traverse position, including coordinates and side status map.
 * @param garden The 2D grid representing the garden layout.
 */
void Region::traverse_from_position(TraversePosition &tp, const std::vector<std::vector<char>> &garden)
{
    visited_garden_plots[tp.y_position][tp.x_position] = true;

    for (auto &pair : tp.side_status_map)
    {
        SideOrientation side_orientation = pair.first;
        SideStatus status = pair.second;

        if (status != SideStatus::AVAILABLE)
            continue;

        // std::cout << "From position (" << tp.x_position << ", " << tp.y_position << "), side " << side_orientation << " is AVAILABLE." << std::endl;

        int new_r = tp.y_position;
        int new_c = tp.x_position;

        switch (side_orientation)
        {
        case SideOrientation::LOWER:
            new_r++;
            break;
        case SideOrientation::UPPER:
            new_r--;
            break;
        case SideOrientation::RIGHT:
            new_c++;
            break;
        case SideOrientation::LEFT:
            new_c--;
            break;
        default:
            break;
        }

        // std::cout << "Checking side in direction " << direction << " to position (" << new_c << ", " << new_r << ") containing " << garden[new_r][new_c] << std::endl;

        auto result = get_traverse_position(new_r, new_c, garden);
        tp.update_side_status(side_orientation, SideStatus::VISITED);
        if (result.has_value() && result.value().value == plant && !visited_garden_plots[new_r][new_c])
        {
            // std::cout << "Can visit side" << std::endl;
            visited_garden_plots[new_r][new_c] = true;

            auto new_tp = result.value();
            // Update the opposite direction as VISITED
            SideOrientation opposite_direction;
            switch (side_orientation)
            {
            case SideOrientation::UPPER:
                opposite_direction = SideOrientation::LOWER;
                break;
            case SideOrientation::LOWER:
                opposite_direction = SideOrientation::UPPER;
                break;
            case SideOrientation::LEFT:
                opposite_direction = SideOrientation::RIGHT;
                break;
            case SideOrientation::RIGHT:
                opposite_direction = SideOrientation::LEFT;
                break;
            default:
                opposite_direction = side_orientation;
                break;
            }
            new_tp.update_side_status(opposite_direction, SideStatus::VISITED);
            traverse_positions.push_back(new_tp);
            // std::cout << "Traversing from position (" << new_tp.x_position << ", " << new_tp.y_position << ")" << std::endl;
            traverse_from_position(new_tp, garden);
        }
    }
}

/**
 * @brief Updates the side status of a given TraversePosition based on its location in the garden.
 *
 * Determines the status of the four sides of the current position (UPPER, LOWER, LEFT, RIGHT):
 * - OUT_OF_BOUNDS if the adjacent plot is outside the garden boundaries.
 * - ADJACENT_TO_OTHER_PLANT_TYPE if the adjacent plot contains a different plant type.
 * - VISITED if the adjacent plot has already been visited.
 * - AVAILABLE if the adjacent plot is of the same plant type and has not been visited.
 *
 * The status for each side is updated in the TraversePosition object using its update_side_status method.
 *
 * @param tp Reference to the TraversePosition whose side statuses will be updated.
 * @param garden 2D vector representing the garden layout, where each cell contains a plant type.
 */
void Region::update_side_status(TraversePosition &tp, const std::vector<std::vector<char>> &garden)
{
    // std::cout << "Updating side status for position (" << tp.x_position << ", " << tp.y_position << ")" << std::endl;

    auto up = tp.y_position - 1;
    auto down = tp.y_position + 1;
    auto left = tp.x_position - 1;
    auto right = tp.x_position + 1;

    if (up < 0)
        tp.update_side_status(SideOrientation::UPPER, SideStatus::OUT_OF_BOUNDS);
    else if (garden[up][tp.x_position] != plant)
        tp.update_side_status(SideOrientation::UPPER, SideStatus::ADJACENT_TO_OTHER_PLANT_TYPE);
    else if (visited_garden_plots[up][tp.x_position])
        tp.update_side_status(SideOrientation::UPPER, SideStatus::VISITED);
    else
        tp.update_side_status(SideOrientation::UPPER, SideStatus::AVAILABLE);

    if (down > (int)garden.size() - 1)
        tp.update_side_status(SideOrientation::LOWER, SideStatus::OUT_OF_BOUNDS);
    else if (garden[down][tp.x_position] != plant)
        tp.update_side_status(SideOrientation::LOWER, SideStatus::ADJACENT_TO_OTHER_PLANT_TYPE);
    else if (visited_garden_plots[down][tp.x_position])
        tp.update_side_status(SideOrientation::LOWER, SideStatus::VISITED);
    else
        tp.update_side_status(SideOrientation::LOWER, SideStatus::AVAILABLE);

    if (left < 0)
        tp.update_side_status(SideOrientation::LEFT, SideStatus::OUT_OF_BOUNDS);
    else if (garden[tp.y_position][left] != plant)
        tp.update_side_status(SideOrientation::LEFT, SideStatus::ADJACENT_TO_OTHER_PLANT_TYPE);
    else if (visited_garden_plots[tp.y_position][left])
        tp.update_side_status(SideOrientation::LEFT, SideStatus::VISITED);
    else
        tp.update_side_status(SideOrientation::LEFT, SideStatus::AVAILABLE);

    if (right > (int)garden[0].size() - 1)
        tp.update_side_status(SideOrientation::RIGHT, SideStatus::OUT_OF_BOUNDS);
    else if (garden[tp.y_position][right] != plant)
        tp.update_side_status(SideOrientation::RIGHT, SideStatus::ADJACENT_TO_OTHER_PLANT_TYPE);
    else if (visited_garden_plots[tp.y_position][right])
        tp.update_side_status(SideOrientation::RIGHT, SideStatus::VISITED);
    else
        tp.update_side_status(SideOrientation::RIGHT, SideStatus::AVAILABLE);
}

/**
 * @brief Constructs a GardenGroup object.
 * @param plant The plant type for the region.
 */
GardenGroup::GardenGroup(char plant)
    : plant(plant) {};


/**
 * @brief Calculates the total fence pricing for all regions in the garden group.
 *
 * Iterates through each region in the garden group and sums up the fence pricing.
 *
 * @param with_sides If true, bases fence pricing on number of sides of the fence; otherwise, number of perimeters.
 * @return The total fence pricing for all regions.
 */
size_t GardenGroup::get_fence_pricing(bool with_sides)
{
    size_t sum = 0;
    for (auto &region : regions)
    {
        sum += region.get_fence_pricing(with_sides);
    }
    return sum;
}
/**
 * @brief Adds a region to the garden group.
 * @param region The region to add.
 */
void GardenGroup::add_region(Region region)
{
    regions.emplace_back(region);
}

/**
 * @brief Constructs a Gardener object and initializes garden groups.
 * @param garden A 2D vector representing the garden layout.
 */
Gardener::Gardener(std::vector<std::vector<char>> garden) : garden_groups(find_garden_groups(garden)) {};

/**
 * @brief Calculates the total fence pricing for all garden groups.
 *
 * Iterates through each garden group and sums up the fence pricing.
 *
 * @param with_sides If true, bases fence pricing on number of sides of the fence; otherwise, number of perimeters.
 * @return The total fence pricing for all garden groups.
 */
size_t Gardener::get_fence_pricing(bool with_sides)
{
    size_t sum = 0;
    for (auto &garden_group : garden_groups)
    {
        sum += garden_group.second.get_fence_pricing(with_sides);
    }
    return sum;
}

/**
 * @brief Identifies and groups regions of the same plant type in the garden.
 *
 * Iterates through each plot in the garden, and for each unvisited plot, it identifies the connected region
 * of the same plant type using depth-first search. Each identified region is added to a corresponding GardenGroup.
 *
 * @param garden A 2D vector representing the garden layout, where each cell contains a character representing a plant type.
 * @return An unordered map where keys are plant types (characters) and values are GardenGroup objects containing regions of that plant type.
 */
std::unordered_map<char, GardenGroup> Gardener::find_garden_groups(const std::vector<std::vector<char>> &garden)
{

    if (visited_garden_plots.empty() || visited_garden_plots.at(0).size() != garden[0].size())
        visited_garden_plots.resize(garden.size(), std::vector<bool>(garden[0].size(), false));

    std::unordered_map<char, GardenGroup> groups;
    groups.reserve(26); // Reserve space for 26 letters (a-z)

    for (int r = 0; r < (int)garden.size(); ++r)
    {
        for (int c = 0; c < (int)garden[r].size(); ++c)
        {
            if (visited_garden_plots[r][c])
                continue;

            // std::cout << "Visiting garden plot (" << r << ", " << c << ") with plant type: " << garden[r][c] << std::endl;
            visited_garden_plots[r][c] = true; // Mark the plot as visited
            char plant_type = garden[r][c];
            if (!groups.contains(plant_type))
            {
                groups.insert(std::make_pair(plant_type, GardenGroup(plant_type)));
            }
            auto it = groups.find(plant_type);
            if (it != groups.end())
            {
                Region plant_region = get_plant_region(r, c, garden);

                // Mark all plots in the region as visited
                for (auto &plot : plant_region.traverse_positions)
                {
                    visited_garden_plots[plot.y_position][plot.x_position] = true;
                }
                it->second.add_region(plant_region);
            }
        }
    }
    return groups;
}

/**
 * @brief Creates a Region object for the plant at the specified starting position in the garden.
 *
 * @param r The row index of the plant in the garden.
 * @param c The column index of the plant in the garden.
 * @param garden A 2D vector representing the garden layout.
 * @return A Region object representing the connected region of the same plant type starting from (r, c).
 */
Region Gardener::get_plant_region(size_t r, size_t c, const std::vector<std::vector<char>> &garden)
{
    return Region(garden[r][c], r, c, garden);
}
