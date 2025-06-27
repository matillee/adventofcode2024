/**
 * @file ceres_search.cpp
 * @brief Processes a word search puzzle to find occurrences of the word "XMAS" in various directions
 *        (horizontal, vertical, diagonal) and counts specific patterns like "MAS" in an X formation.
 *
 * This program reads a puzzle input file, parses it into a 2D character array, and provides functions
 * to count the number of times "XMAS" appears horizontally, vertically, and diagonally, as well as
 * the number of "MAS" patterns in an X formation.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>

/**
 * @class PuzzleInput
 * @brief Handles reading and parsing of the word search puzzle input file.
 */
class PuzzleInput
{
public:
    /**
     * @brief Constructs a PuzzleInput object and reads the file content.
     * @param filename The path to the input file. Defaults to "../puzzle_input".
     */
    PuzzleInput(const std::string &filename = "../puzzle_input")
    {
        file_content = read_file(filename);
    }

    /**
     * @brief Parses the file content into a 2D array of characters.
     * @return A 2D vector of characters representing the puzzle.
     * @throws std::runtime_error if the file is empty.
     */
    std::vector<std::vector<char>> parse_input()
    {
        if (file_content.empty())
        {
            throw std::runtime_error("No valid content in the file.");
        }
        std::vector<std::vector<char>> array;
        for (const auto &line : file_content)
        {
            array.emplace_back(line.begin(), line.end());
        }
        return array;
    }

private:
    std::vector<std::string> file_content;

    /**
     * @brief Reads the content of the input file.
     * @param filename The path to the input file.
     * @return A vector of strings, each representing a line in the file.
     * @throws std::runtime_error if the file does not exist.
     */
    std::vector<std::string> read_file(const std::string &filename)
    {
        std::vector<std::string> lines;
        std::ifstream infile(filename);
        if (!infile)
        {
            throw std::runtime_error("Error: The file " + filename + " does not exist.");
        }
        std::string line;
        while (std::getline(infile, line))
        {
            if (!line.empty())
                lines.push_back(line);
        }
        return lines;
    }
};

/**
 * @class CeresSearch
 * @brief Handles searching for patterns in the word search puzzle.
 */
class CeresSearch
{
public:
    /**
     * @brief Counts the number of times "XMAS" appears horizontally (left-to-right and right-to-left).
     * @param array The 2D character array representing the puzzle.
     * @return The count of "XMAS" horizontally.
     */
    size_t count_xmas_horizontal(const std::vector<std::vector<char>> &array)
    {
        size_t count = 0;
        for (const auto &row : array)
        {
            // Convert the row vector to a string using iterators and string constructor which
            // constructs a string with the contents of the range [first, last)
            std::string row_str(row.begin(), row.end());
            count += count_substring(row_str, "XMAS");
            std::reverse(row_str.begin(), row_str.end());
            count += count_substring(row_str, "XMAS");
        }
        // std::cout << "Counted " << count << " XMAS in horizontal direction." << std::endl;
        return count;
    }

    /**
     * @brief Counts the number of times "XMAS" appears vertically (top-to-bottom and bottom-to-top).
     * @param array The 2D character array representing the puzzle.
     * @return The count of "XMAS" vertically.
     */
    size_t count_xmas_vertical(const std::vector<std::vector<char>> &array)
    {
        size_t count = 0;
        size_t row_size = array.size();
        size_t col_size = array[0].size();
        for (size_t col = 0; col < col_size; ++col)
        {
            std::string col_str;
            for (size_t row = 0; row < row_size; ++row)
            {
                col_str += array[row][col];
            }
            count += count_substring(col_str, "XMAS");
            std::reverse(col_str.begin(), col_str.end());
            count += count_substring(col_str, "XMAS");
        }
        // std::cout << "Counted " << count << " XMAS in vertical direction." << std::endl;
        return count;
    }

    /**
     * @brief Counts the number of times "XMAS" appears diagonally (all four diagonal directions).
     * @param array The 2D character array representing the puzzle.
     * @return The count of "XMAS" diagonally.
     */
    size_t count_xmas_diagonal(const std::vector<std::vector<char>> &array)
    {
        size_t count = 0;
        int row_size = array.size();
        int col_size = array[0].size();

        // Top-left to bottom-right
        for (int i = 0; i < row_size; ++i)
        {
            for (int j = 0; j < col_size; ++j)
            {
                if (i + 3 < row_size && j + 3 < col_size)
                {
                    std::string diag;
                    for (int k = 0; k < 4; ++k)
                        diag += array[i + k][j + k];
                    count += count_substring(diag, "XMAS");
                    std::reverse(diag.begin(), diag.end());
                    count += count_substring(diag, "XMAS");
                }
            }
        }
        // Top-right to bottom-left
        for (int i = 0; i < row_size; ++i)
        {
            for (int j = 0; j < col_size; ++j)
            {
                if (i + 3 < row_size && j - 3 >= 0)
                {
                    std::string diag;
                    for (int k = 0; k < 4; ++k)
                        diag += array[i + k][j - k];
                    count += count_substring(diag, "XMAS");
                    std::reverse(diag.begin(), diag.end());
                    count += count_substring(diag, "XMAS");
                }
            }
        }
        // std::cout << "Counted " << count << " XMAS in diagonal direction." << std::endl;
        return count;
    }

    /**
     * @brief Counts the total number of times "XMAS" appears in any direction.
     * @param array The 2D character array representing the puzzle.
     * @return The total count of "XMAS" in all directions.
     */
    size_t count_xmas_all(const std::vector<std::vector<char>> &array)
    {
        // std::cout << "Parsing array with " << array.size() << " rows and " << array[0].size() << " columns." << std::endl;
        size_t total_count = count_xmas_horizontal(array) + count_xmas_vertical(array) + count_xmas_diagonal(array);
        // std::cout << "Total XMAS count: " << total_count << std::endl;
        return total_count;
    }

    /**
     * @brief Counts the number of "MAS" patterns in an X formation centered on 'A'.
     * @param array The 2D character array representing the puzzle.
     * @return The count of "MAS" in X formation.
     */
    size_t count_mas_in_x_formation(const std::vector<std::vector<char>> &array)
    {
        size_t count = 0;
        int row_size = array.size();
        int col_size = array[0].size();

        for (int i = 1; i < row_size - 1; ++i)
        {
            for (int j = 1; j < col_size - 1; ++j)
            {
                if (array[i][j] == 'A')
                {
                    // Check all possible X formations
                    if (
                        (array[i - 1][j - 1] == 'M' && array[i - 1][j + 1] == 'M' &&
                         array[i + 1][j - 1] == 'S' && array[i + 1][j + 1] == 'S') ||
                        (array[i - 1][j - 1] == 'S' && array[i - 1][j + 1] == 'S' &&
                         array[i + 1][j - 1] == 'M' && array[i + 1][j + 1] == 'M') ||
                        (array[i - 1][j - 1] == 'M' && array[i - 1][j + 1] == 'S' &&
                         array[i + 1][j - 1] == 'M' && array[i + 1][j + 1] == 'S') ||
                        (array[i - 1][j - 1] == 'S' && array[i - 1][j + 1] == 'M' &&
                         array[i + 1][j - 1] == 'S' && array[i + 1][j + 1] == 'M'))
                    {
                        // std::cout << "Found MAS at (" << i << ", " << j << ") in X formation." << std::endl;
                        count += 1;
                    }
                }
            }
        }
        // std::cout << "Counted " << count << " MAS in X formation." << std::endl;
        return count;
    }

private:
    /**
     * @brief Counts the number of times a substring appears in a string (overlapping allowed).
     * @param original_str The string to search in.
     * @param wanted_substr The substring to search for.
     * @return The count of occurrences of wanted_substr in original_str.
     */
    size_t count_substring(const std::string &original_str, const std::string &wanted_substr)
    {
        size_t count = 0;
        size_t pos = 0;

        // find returns position of the first character of the found substring or npos if no such substring is found.
        while ((pos = original_str.find(wanted_substr, pos)) != std::string::npos)
        {
            ++count;
            ++pos;
        }
        return count;
    }
};

/**
 * @brief Main function. Reads the puzzle input, parses it, and prints the results for both parts.
 */
int main()
{
    try
    {
        PuzzleInput input_handler;
        std::vector<std::vector<char>> puzzle_array = input_handler.parse_input();

        CeresSearch ceres_search;
        std::cout << "Part 1:" << std::endl;
        std::cout << ceres_search.count_xmas_all(puzzle_array) << std::endl;

        std::cout << "Part 2:" << std::endl;
        std::cout << ceres_search.count_mas_in_x_formation(puzzle_array) << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}