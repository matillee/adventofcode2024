/**
 * @file manager.cpp
 * @brief Implements the ManagerClass for plutonian pebbles input and delegating pebble changing logic.
 */
#include "plutonian_pebbles.hpp"

/**
 * @brief Constructs a ManagerClass and reads the hiking map from file.
 * @param input_file_name The path to the input file.
 */
ManagerClass::ManagerClass(const std::string &input_file_name)
    : starting_pebble_order(read_input(input_file_name)) {};

/**
 * @brief Reads the starting pebble order from the input file.
 *
 * Reads all non-whitespace characters from the file and returns them as a 2D vector of size_t integers.
 * @param filename The path to the input file.
 * @return A vector of size_t integers representing the starting pebble order.
 * @throws std::runtime_error if the file does not exist or is empty.
 */
std::vector<size_t> ManagerClass::read_input(const std::string &filename)
{
    std::vector<size_t> order;
    std::ifstream infile(filename);
    if (!infile)
        throw std::runtime_error("Error: The file " + filename + " does not exist.");

    std::string line;
    while (std::getline(infile, line, ' '))
    {
        if (!line.empty())
        {
            order.emplace_back(stoull(line));

        }
    }
    if (order.empty())
    {
        throw std::runtime_error("Error: The file " + filename + " is empty or invalid.");
    }
    return order;
}

/**
 * @brief Returns the number of pebbles after blinking calculated by the plutonian pebble transformer.
 * @return The number of pebbles as a size_t integer.
 */
size_t ManagerClass::get_number_of_pebbles(size_t number_of_blinks)
{
    PlutonianPebbleTransformer pebble_transformer(starting_pebble_order);
    return pebble_transformer.get_number_of_pebbles_after_blinking(number_of_blinks);
}
