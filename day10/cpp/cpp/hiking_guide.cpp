/**
 * @file hiking_guide.cpp
 * @brief Implements classes and methods for hiking map analysis, trailhead scoring, and DFS traversal.
 */
#include "hiking_guide.hpp"

/**
 * @brief Constructs a Position object.
 * @param x The x (#columns) position.
 * @param y The y (#rows) position.
 * @param value The content of the position.
 */
Position::Position(int x, int y, size_t value)
    : x_position(x), y_position(y), value(value) {};

/**
 * @brief Equality operator for Position.
 * @param other The other Position to compare.
 * @return True if positions are equal.
 */
bool Position::operator==(const Position &other) const
{
    return x_position == other.x_position && y_position == other.y_position && value == other.value;
};

/**
 * @brief Constructs a Trailhead object and initializes reachable ending positions.
 * @param map The hiking map as a 2D vector.
 * @param starting_pos The starting position for the trailhead.
 * @param ending_height The target ending height for the trail.
 * @param height_increment The increment for each step in height.
 */
Trailhead::Trailhead(const std::vector<std::vector<size_t>> &map, const Position &starting_pos, size_t ending_height, size_t height_increment)
    : map(map),
      starting_pos(starting_pos),
      ending_height(ending_height),
      height_increment(height_increment),
      hike_trails_dfs(HikeTrailsDFS(map, ending_height, height_increment))
{
    reachable_ending_positions = hike_trails_dfs.get_reachable_ending_positions(starting_pos);
}

/**
 * @brief Returns the score for this trailhead (number of reachable ending positions).
 * @return The score as a size_t integer.
 */
size_t Trailhead::get_score()
{
    return reachable_ending_positions.size();
}

/**
 * @brief Returns the sum of ratings for all trailheads from this starting position.
 * @return The sum of ratings as a size_t integer.
 */
size_t Trailhead::get_rating_of_trailheads()
{
    auto rating_of_trail_heads = hike_trails_dfs.get_rating_of_trailheads(starting_pos);
    size_t sum = 0;
    for (auto &rating : rating_of_trail_heads)
    {
        sum += rating.second;
    }
    return sum;
}

/**
 * @brief Constructs a HikeGuide object with the given map.
 * @param map The hiking map as a 2D vector.
 */
HikeGuide::HikeGuide(const std::vector<std::vector<size_t>> &map) : map(map)
{
    find_trail_heads();
    std::cout << "Trail heads " << trail_heads.size() << std::endl;
};

/**
 * @brief Returns the total score for all trailheads in the map.
 * @return The total score as a size_t integer.
 */
size_t HikeGuide::get_score()
{
    size_t sum = 0;
    for (auto &trail_head : trail_heads)
    {
        sum += trail_head.get_score();
    }
    return sum;
}

/**
 * @brief Returns the sum of ratings for all trailheads in the map.
 * @return The sum of ratings as a size_t integer.
 */
size_t HikeGuide::get_sum_rating_of_all_trailheads()
{
    size_t sum = 0;
    for (auto &trail_head : trail_heads)
    {
        sum += trail_head.get_rating_of_trailheads();
    }
    return sum;
}

/**
 * @brief Finds all trailheads (positions with value 0) in the map and initializes them.
 */
void HikeGuide::find_trail_heads()
{
    for (int i = 0; i < (int)map.size(); ++i)
    {
        for (int j = 0; j < (int)map[i].size(); ++j)
        {
            if (map[i][j] == 0)
            {
                trail_heads.push_back(Trailhead(map, Position(j, i, 0), 9, 1));
            }
        }
    }
}

/**
 * @brief Constructs a HikeTrailsDFS object for DFS traversal of the hiking map.
 * @param grid The hiking map as a 2D vector.
 * @param stop_value The target value to stop DFS.
 * @param height_increment The increment for each step in height.
 */
HikeTrailsDFS::HikeTrailsDFS(const std::vector<std::vector<size_t>> &grid, size_t stop_value, size_t height_increment)
    : grid(grid),
      rows(grid.size()),
      cols(grid[0].size()),
      stop_value(stop_value),
      height_increment(height_increment),
      processed_dfs(false),
      visited(rows, std::vector<bool>(cols, false)) {}

/**
 * @brief Returns the set of reachable ending positions from a given trailhead using DFS.
 * @param trail_head The starting position for DFS.
 * @return An unordered_set of Position objects.
 */
std::unordered_set<Position> HikeTrailsDFS::get_reachable_ending_positions(Position trail_head)
{
    dfs(trail_head.y_position, trail_head.x_position, trail_head.value);
    processed_dfs = true;
    return reachable_ending_positions;
}

/**
 * @brief Returns the rating of all trailheads from a given starting position using DFS.
 * @param trail_head The starting position for DFS.
 * @return An unordered_map of Position to rating.
 */
std::unordered_map<Position, size_t> HikeTrailsDFS::get_rating_of_trailheads(Position trail_head)
{
    if (!processed_dfs)
    {
        dfs(trail_head.y_position, trail_head.x_position, trail_head.value);
        processed_dfs = true;
    }

    return rating_of_trailheads;
}

/**
 * @brief Performs DFS traversal from a given position and target value.
 * @param r The row index.
 * @param c The column index.
 * @param target_value The target value for the current step.
 */
void HikeTrailsDFS::dfs(size_t r, size_t c, size_t target_value)
{
    if (target_value == stop_value && grid[r][c] == stop_value)
    {
        auto ending_position = Position(c, r, grid[r][c]);
        reachable_ending_positions.emplace(ending_position);
        if (!rating_of_trailheads.contains(ending_position))
        {
            rating_of_trailheads.insert({ending_position, 1});
        }
        else
            rating_of_trailheads[ending_position] += 1;
        return;
    }
    if (r >= rows || c >= cols || grid[r][c] != target_value)
        return;

    // Hiking trails never include diagonal steps - only up, down, left, or right (from the perspective of the map).
    if (r > 0)
        dfs(r - 1, c, target_value + height_increment); // Up
    if (r + 1 < rows)
        dfs(r + 1, c, target_value + height_increment); // Down
    if (c > 0)
        dfs(r, c - 1, target_value + height_increment); // Left
    if (c + 1 < cols)
        dfs(r, c + 1, target_value + height_increment); // Right
}