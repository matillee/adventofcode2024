/**
 * @file guard_gallivant.cpp
 * @brief Simulates a guard patrolling a 2D map, tracking visited positions and detecting loops.
 */
#include "guard_gallivant.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <unordered_set>
#include <map>
#include <algorithm>
// #include <thread>

/**
 * @brief Constructs a GuardMovement object.
 * @param x The x (#columns) position.
 * @param y The y (#rows) position.
 * @param dir The direction character ('^', 'v', '<', '>').
 */
GuardMovement::GuardMovement(const int &x, const int &y, const char &dir)
    : direction(dir), x_position(x), y_position(y) {}

/**
 * @brief Equality operator for GuardMovement.
 * @param other The other GuardMovement to compare.
 * @return True if position and direction are equal.
 */
bool GuardMovement::operator==(const GuardMovement &other) const
{
    return direction == other.direction && x_position == other.x_position && y_position == other.y_position;
}

/**
 * @brief Output stream operator for GuardMovement.
 * @param os The output stream.
 * @param gm The GuardMovement to print.
 * @return The output stream.
 */
std::ostream &operator<<(std::ostream &os, const GuardMovement &gm)
{
    os << "GuardMovement(x=" << gm.x_position << ", y=" << gm.y_position << ", dir=" << gm.direction << ")";
    return os;
}

/**
 * @brief Constructs a Position object.
 * @param x The x (#columns) position.
 * @param y The y (#rows) position.
 */
Position::Position(const int &x, const int &y)
    : x_position(x), y_position(y) {}

/**
 * @brief Equality operator for Position.
 * @param other The other Position to compare.
 * @return True if positions are equal.
 */
bool Position::operator==(const Position &other) const
{
    return x_position == other.x_position && y_position == other.y_position;
}

/**
 * @brief Returns a C-string describing the exception.
 * @return The exception message.
 */
char *OutOfBoundsException::what()
{
    return (char *)"Guard is out of bounds";
}

/**
 * @brief Computes the next guard movement based on the current state and map.
 * @param current The current GuardMovement.
 * @param map The 2D map.
 * @return The next GuardMovement.
 * @throws OutOfBoundsException if the guard moves out of bounds.
 * @throws std::invalid_argument if the direction is invalid.
 */
GuardMovement GuardBehaviour::patrol_area(const GuardMovement &current, const std::vector<std::vector<char>> &map)
{
    auto it = direction_map().find(current.direction);
    if (it == direction_map().end())
    {
        throw std::invalid_argument("Invalid direction");
    }

    int dx = it->second.first;
    int dy = it->second.second;
    int new_x = current.x_position + dx;
    int new_y = current.y_position + dy;

    if (guard_is_out_of_bounds(new_x, new_y, map))
    {
        throw OutOfBoundsException();
    }

    if (map[new_y][new_x] == '#')
    {
        return GuardMovement(current.x_position, current.y_position, handle_obstacle_encounter(current.direction));
    }

    return GuardMovement(new_x, new_y, current.direction);
}

/**
 * @brief Returns the static direction map.
 * @return A map from direction char to (dx, dy) pair.
 */
const std::map<char, std::pair<int, int>> &GuardBehaviour::direction_map()
{
    static const std::map<char, std::pair<int, int>> dir_map = {
        {'^', {0, -1}}, // up
        {'v', {0, 1}},  // down
        {'<', {-1, 0}}, // left
        {'>', {1, 0}}   // right
    };
    return dir_map;
}

/**
 * @brief Handles obstacle encounter by turning the guard 90 degrees clockwise.
 * @param direction The current direction.
 * @return The new direction after turning.
 * @throws std::invalid_argument if the direction is invalid.
 */
char GuardBehaviour::handle_obstacle_encounter(const char &direction)
{
    static const std::vector<char> directions = {'^', '>', 'v', '<'};
    auto it = std::find(directions.begin(), directions.end(), direction);
    if (it != directions.end())
    {
        int idx = std::distance(directions.begin(), it);
        return directions[(idx + 1) % directions.size()];
    }
    else
    {
        throw std::invalid_argument("Invalid guard direction");
    }
}

/**
 * @brief Checks if the given position is out of bounds for the map.
 * @param new_x The x (#columns) position.
 * @param new_y The y (#rows) position.
 * @param map The 2D map.
 * @return True if out of bounds, false otherwise.
 */
bool GuardBehaviour::guard_is_out_of_bounds(const int &new_x, const int &new_y, const std::vector<std::vector<char>> &map)
{
    if (new_x < 0 or (size_t) new_x > map.size() - 1)
    {
        return true;
    }
    if (new_y < 0 or (size_t) new_y > map[0].size() - 1)
    {
        return true;
    }
    return false;
}

/**
 * @brief Constructs a GuardSimulation with the given map and initial guard movement.
 * @param starting_map The 2D map.
 * @param initial_guard_movement The guard's starting position and direction.
 */
GuardSimulation::GuardSimulation(std::vector<std::vector<char>> starting_map, GuardMovement initial_guard_movement)
    : map(std::move(starting_map)),
      initial_guard_movement(std::move(initial_guard_movement))
{
    visited_positions_with_direction.insert(this->initial_guard_movement);
    visited_positions.insert(Position(this->initial_guard_movement.x_position, this->initial_guard_movement.y_position));
}

/**
 * @brief Returns the set of all positions patrolled by the guard.
 * @return An unordered_set of Position objects.
 */
std::unordered_set<Position> GuardSimulation::get_patrolled_area()
{
    guard_patrols_area_and_leaves_map();
    // print_patrolled_area(current_guard_movement);
    return visited_positions;
}

/**
 * @brief Determines if the simulation results in a loop.
 * @return True if a loop is detected, false otherwise.
 */
bool GuardSimulation::results_in_loop()
{
    return !guard_patrols_area_and_leaves_map();
}

/**
 * @brief Simulates the guard's patrol, tracking visited positions and detecting loops.
 * @return True if the guard leaves the map, false if a loop is detected.
 */
bool GuardSimulation::guard_patrols_area_and_leaves_map()
{
    GuardMovement current_guard_movement = initial_guard_movement;
    visited_positions_with_direction.clear();
    visited_positions_with_direction.insert(current_guard_movement);

    while (true)
    {
        try
        {
            // std::cout << current_guard_movement << "\n";
            GuardMovement next_guard_movement = GuardBehaviour::patrol_area(current_guard_movement, map);

            if (visited_positions_with_direction.count(next_guard_movement))
            {
                // Loop detected if this position+direction is already visited
                break;
            }

            visited_positions_with_direction.insert(next_guard_movement);
            visited_positions.insert(Position(next_guard_movement.x_position, next_guard_movement.y_position));
            current_guard_movement = next_guard_movement;
        }
        catch (const OutOfBoundsException &e)
        {
            return true;
        }
    }

    return false;
}

/**
 * @brief Prints the patrolled area, marking the guard's current position and visited positions.
 * @param current_guard_movement The current GuardMovement.
 */
void GuardSimulation::print_patrolled_area(GuardMovement current_guard_movement)
{

    for (size_t y = 0; y < map.size(); y++)
    {
        for (size_t x = 0; x < map[0].size(); x++)
        {
            if ((size_t)current_guard_movement.x_position == x && (size_t)current_guard_movement.y_position == y)
                std::cout << current_guard_movement.direction;
            else if (visited_positions.contains(Position(x, y)))
                std::cout << '~';
            else
                std::cout << map[y][x];
        }
        std::cout << "\n";
    }
}

/**
 * @brief Constructs a ManagerClass, reads the map, and finds the guard's starting position.
 * @param input_file_name The path to the input file.
 * @throws std::runtime_error if the map is empty or invalid.
 */
ManagerClass::ManagerClass(const std::string &input_file_name)
    : starting_map(read_input(input_file_name)),
      initial_guard_movement(find_guard_in_map(starting_map))
{
    if (starting_map.empty())
    {
        throw std::runtime_error("Map content is not valid, empty.");
    }
}

/**
 * @brief Returns all positions that, if obstructed, would cause the guard to loop.
 * @return An unordered_set of Position objects.
 */
std::unordered_set<Position> ManagerClass::get_all_possible_obstructions_to_create_guard_loops()
{
    // Get all positions patrolled by the guard
    std::unordered_set<Position> visited_positions = get_patrolled_area();
    std::unordered_set<Position> loop_positions;

    // TODO: Add threadpool here
    for (const auto &pos : visited_positions)
    {
        // Make a copy of the map
        std::vector<std::vector<char>> map_with_obstacle = starting_map;
        // Place an obstacle at the visited position
        map_with_obstacle[pos.y_position][pos.x_position] = '#';
        // Run the simulation with the new map
        GuardSimulation sim(map_with_obstacle, initial_guard_movement);
        if (sim.results_in_loop())
        {
            loop_positions.insert(pos);
        }
    }
    return loop_positions;
}

/**
 * @brief Returns the number of positions that would cause a loop if obstructed.
 * @return The number of such positions.
 */
size_t ManagerClass::get_number_of_obstructions_for_guard_loops()
{
    return get_all_possible_obstructions_to_create_guard_loops().size();
}

/**
 * @brief Returns the set of all positions patrolled by the guard.
 * @return An unordered_set of Position objects.
 */
std::unordered_set<Position> ManagerClass::get_patrolled_area()
{
    return GuardSimulation(starting_map, initial_guard_movement).get_patrolled_area();
}

/**
 * @brief Returns the number of unique positions patrolled by the guard.
 * @return The number of unique positions.
 */
size_t ManagerClass::get_number_of_patrolled_positions()
{
    return get_patrolled_area().size();
}

/**
 * @brief Reads the content of the input file.
 * @param filename The path to the input file.
 * @return A 2D vector of characters representing the puzzle.
 * @throws std::runtime_error if the file does not exist.
 */
std::vector<std::vector<char>> ManagerClass::read_input(const std::string &filename)
{

    std::vector<std::vector<char>> array;
    std::ifstream infile(filename);
    if (!infile)
    {
        throw std::runtime_error("Error: The file " + filename + " does not exist.");
    }
    std::string line;
    while (std::getline(infile, line))
    {
        if (!line.empty())
            array.emplace_back(line.begin(), line.end());
    }
    return array;
}

/**
 * @brief Finds the guard's starting position and direction in the map.
 * @param map The 2D map.
 * @return The GuardMovement representing the guard's start.
 * @throws std::invalid_argument if the guard is not found.
 */
GuardMovement ManagerClass::find_guard_in_map(std::vector<std::vector<char>> map)
{
    for (size_t y = 0; y < map.size(); y++) // row (number of rows = y)
    {
        for (size_t x = 0; x < map[0].size(); x++) // column (number of columns = x)
        {
            if (std::string("^<>v").find(map[y][x]) != std::string::npos)
            {
                return GuardMovement(x, y, map[y][x]);
            }
        }
    }
    throw std::invalid_argument("Guard starting position not found on the map.");
}
