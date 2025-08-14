/**
 * @file manager.cpp
 * @brief Implements the ManagerClass for reading hiking map input and delegating scoring and rating logic.
 */
#include "hiking_guide.hpp"

/**
 * @brief Constructs a ManagerClass and reads the hiking map from file.
 * @param input_file_name The path to the input file.
 */
ManagerClass::ManagerClass(const std::string &input_file_name)
    : map(read_input(input_file_name)), hike_guide(map) {};

/**
 * @brief Reads the hiking map from the input file.
 *
 * Reads all non-whitespace characters from the file and returns them as a 2D vector of size_t integers.
 * @param filename The path to the input file.
 * @return A 2D vector of size_t integers representing the hiking map.
 * @throws std::runtime_error if the file does not exist or is empty.
 */
std::vector<std::vector<size_t>> ManagerClass::read_input(const std::string &filename)
{
    std::vector<size_t> converted_line;
    std::vector<std::vector<size_t>> hiking_map;
    std::ifstream infile(filename);
    if (!infile)
        throw std::runtime_error("Error: The file " + filename + " does not exist.");

    std::string line;
    while (std::getline(infile, line))
    {
        if (!line.empty())
        {
            converted_line.reserve(line.size());
            std::transform(line.begin(), line.end(), std::back_inserter(converted_line),
                           [&](char c)
                           { return convert_char_to_size_t(c); });

            hiking_map.emplace_back(std::move(converted_line));
        }
    }
    if (hiking_map.empty())
    {
        throw std::runtime_error("Error: The file " + filename + " is empty or invalid.");
    }
    return hiking_map;
}

/**
 * @brief Converts a character to its corresponding size_t value.
 * @param c The character to convert (ASCII digit).
 * @return The size_t integer value.
 */
size_t ManagerClass::convert_char_to_size_t(char c)
{
    return (size_t)c - 48;
}

/**
 * @brief Returns the score calculated by the hiking guide.
 * @return The score as a size_t integer.
 */
size_t ManagerClass::get_score()
{
    return hike_guide.get_score();
}

/**
 * @brief Returns the sum of ratings for all trailheads as calculated by the hiking guide.
 * @return The sum of ratings as a size_t integer.
 */
size_t ManagerClass::get_sum_rating_of_all_trailheads()
{
    return hike_guide.get_sum_rating_of_all_trailheads();
}