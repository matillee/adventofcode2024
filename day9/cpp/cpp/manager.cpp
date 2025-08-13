#include "file_formatter.hpp"

/**
 * @brief Constructs a ManagerClass and reads the disk map from file.
 * @param input_file_name The path to the input file.
 */
ManagerClass::ManagerClass(const std::string &input_file_name)
{
    original_diskmap = read_input(input_file_name);
}

/**
 * @brief Reads the disk map from the input file.
 *
 * Reads all non-whitespace characters from the file and returns them as a vector of characters.
 * @param filename The path to the input file.
 * @return A vector of characters representing the disk map.
 * @throws std::runtime_error if the file does not exist or is empty.
 */
std::vector<char> ManagerClass::read_input(const std::string &filename)
{
    std::vector<char> diskmap;
    std::ifstream infile(filename);
    if (!infile)
    {
        throw std::runtime_error("Error: The file " + filename + " does not exist.");
    }
    std::string line;
    while (std::getline(infile, line))
    {
        for (char c : line)
        {
            if (c != '\n' && c != '\r' && c != ' ')
            {
                diskmap.push_back(c);
            }
        }
    }
    if (diskmap.empty())
    {
        throw std::runtime_error("Error: The file " + filename + " is empty or invalid.");
    }
    return diskmap;
}

/**
 * @brief Returns the checksum of the disk map using FileFormatter.
 * @return The calculated checksum value.
 */
size_t ManagerClass::get_checksum()
{
    FileFormatter file_formatter(original_diskmap, true);
    return file_formatter.get_checksum();
}

/**
 * @brief Returns the checksum of the disk map using FileFormatter and rearraning whole files.
 * @return The calculated checksum value.
 */
size_t ManagerClass::get_checksum_for_whole_files()
{
    FileFormatter file_formatter(original_diskmap, false);
    return file_formatter.get_checksum();
}