/**
 * @file manager.cpp
 * @brief Implements the ManagerClass for reading hiking map input and delegating scoring and rating logic.
 */
#include "garden_groups.hpp"

/**
 * @brief Constructs a ManagerClass and reads the hiking map from file.
 * @param input_file_name The path to the input file.
 */
ManagerClass::ManagerClass(const std::string &input_file_name)
    : garden(read_input(input_file_name)), gardener(Gardener(garden)){};

/**
 * @brief Reads the hiking map from the input file.
 *
 * Reads all non-whitespace characters from the file and returns them as a 2D vector of size_t integers.
 * @param filename The path to the input file.
 * @return A 2D vector of size_t integers representing the hiking map.
 * @throws std::runtime_error if the file does not exist or is empty.
 */
std::vector<std::vector<char>> ManagerClass::read_input(const std::string &filename)
{
    std::vector<std::vector<char>> garden;
    std::ifstream infile(filename);
    if (!infile)
        throw std::runtime_error("Error: The file " + filename + " does not exist.");

    std::string line;
    while (std::getline(infile, line))
    {
        if (!line.empty())
        {
            garden.emplace_back(line);

        }
    }
    if (garden.empty())
    {
        throw std::runtime_error("Error: The file " + filename + " is empty or invalid.");
    }
    return garden;
}

/**
 * @brief Returns the fence pricing calculated by the gardener.
 * @return The fence pricing as a size_t integer.
 */
size_t ManagerClass::get_fence_pricing()
{
    return gardener.get_fence_pricing();
}
