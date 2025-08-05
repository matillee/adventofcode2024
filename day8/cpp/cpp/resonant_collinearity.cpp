// How many unique locations within the bounds of the map contain an antinode?

#include "resonant_collinearity.hpp"

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
 * @brief Constructs a ManagerClass and reads the grid from file.
 * @param filename The path to the input file.
 */
ManagerClass::ManagerClass(const std::string &filename)
{
    grid = read_input(filename);
}

/**
 * @brief Returns a set of unique antinode positions in the grid.
 * @return An unordered_set of Position objects.
 */
std::unordered_set<Position> ManagerClass::get_unique_antinode_positions()
{
    ResonantCollinearity resonant_collinearity(grid);
    return resonant_collinearity.get_unique_antinode_positions();
}

/**
 * @brief Returns a set of unique antinode positions in the grid where resonant harmonics have been taken into account.
 * @return An unordered_set of Position objects.
 */
std::unordered_set<Position> ManagerClass::get_unique_antinode_positions_with_resonant_harmonics()
{
    ResonantCollinearity resonant_collinearity(grid, true);
    return resonant_collinearity.get_unique_antinode_positions();
}

/**
 * @brief Returns the number of unique antinode positions in the grid.
 * @return The number of unique antinode positions.
 */
size_t ManagerClass::get_number_of_unique_antinode_positions()
{
    ResonantCollinearity resonant_collinearity(grid);
    return resonant_collinearity.get_number_of_unique_antinode_positions();
}

/**
 * @brief Reads the content of the input file.
 * @param filename The path to the input file.
 * @return A 2D vector of characters representing the grid.
 * @throws std::runtime_error if the file does not exist or is empty.
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
    if (array.empty())
    {
        throw std::runtime_error("Error: The file " + filename + " is empty or invalid.");
    }
    return array;
}

/**
 * @brief Constructs a ResonantCollinearity object and processes frequencies in the grid.
 * @param grid The 2D grid of characters.
 * @param resonant_harmonics Whether to consider resonant harmonics.
 */
ResonantCollinearity::ResonantCollinearity(const std::vector<std::vector<char>> &grid, bool resonant_harmonics)
{
    std::cout << "Processing grid of size: " << grid.size() << "x" << grid[0].size() << std::endl;
    // Initialize the ResonantCollinearity with the provided grid
    this->unique_antinode_positions.clear();
    this->frequencies.clear();

    process_frequencies(grid, resonant_harmonics);
}

/**
 * @brief Processes the grid to find unique frequencies and their antinode positions.
 * @param grid The 2D grid of characters.
 * @param resonant_harmonics Whether to consider resonant harmonics.
 */
void ResonantCollinearity::process_frequencies(const std::vector<std::vector<char>> &grid, bool resonant_harmonics)
{
    std::unordered_set<char> unique_frequencies;
    // Process the grid to find unique frequencies and their antinode positions
    for (const auto &row : grid)
    {
        for (char cell : row)
        {
            if (cell != '.' && cell != '#')
            {
                unique_frequencies.insert(cell);
            }
        }
    }

    // Create Frequency objects for each unique frequency
    for (char frequency_char : unique_frequencies)
    {
        frequencies.emplace(Frequency(frequency_char, grid));
    }
    //std::cout << "Found " << frequencies.size() << " unique frequencies in the grid." << std::endl;

    // Collect all unique antinode positions from all frequencies
    std::unordered_set<Position> antinode_positions;
    for (const auto &frequency : frequencies)
    {
        /*  if (resonant_harmonics)
             auto antinode_positions = frequency.get_unique_antinode_positions_with_resonant_harmonics(grid);
         else */
        auto antinode_positions = frequency.get_antinode_positions(grid, resonant_harmonics);
        unique_antinode_positions.insert(antinode_positions.begin(), antinode_positions.end());
    }

    // Debug output for antinode positions
    for (int i = 0; i < (int)grid.size(); ++i)
    {
        for (int j = 0; j < (int)grid[i].size(); ++j)
        {
            if (unique_antinode_positions.find(Position(j, i)) != unique_antinode_positions.end())
            {
                std::cout << "# ";
            }
            else
            {
                std::cout << grid[i][j] << " ";
            }
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Returns the set of unique antinode positions found in the grid.
 * @return An unordered_set of Position objects.
 */
std::unordered_set<Position> ResonantCollinearity::get_unique_antinode_positions()
{
    return unique_antinode_positions;
}

/**
 * @brief Returns the number of unique antinode positions found in the grid.
 * @return The number of unique antinode positions.
 */
size_t ResonantCollinearity::get_number_of_unique_antinode_positions()
{
    return unique_antinode_positions.size();
}

/**
 * @brief Constructs a Frequency object and finds all positions for the frequency in the grid.
 * @param frequency_char The character representing the frequency.
 * @param grid The 2D grid of characters.
 */
Frequency::Frequency(char frequency_char, const std::vector<std::vector<char>> &grid)
    : frequency_char(frequency_char)
{
    find_frequency_positions(grid);
}

/**
 * @brief Returns the character representing the frequency.
 * @return The frequency character.
 */
char Frequency::get_frequency_char() const
{
    return frequency_char;
}

/**
 * @brief Finds all positions of the frequency in the grid and stores them.
 * @param grid The 2D grid of characters.
 */
void Frequency::find_frequency_positions(const std::vector<std::vector<char>> &grid)
{

    for (int i = 0; i < (int)grid.size(); ++i)
    {
        for (int j = 0; j < (int)grid[i].size(); ++j)
        {
            if (grid[i][j] == frequency_char)
            {
                frequency_positions.insert(Position(j, i));
            }
        }
    }

    //std::cout << "Found " << frequency_positions.size() << " positions for frequency: " << frequency_char << std::endl;
}

/**
 * @brief Returns the set of antinode positions for this frequency in the grid.
 * @param grid The 2D grid of characters.
 * @param resonant_harmonics Whether to consider resonant harmonics.
 * @return An unordered_set of Position objects.
 */
std::unordered_set<Position> Frequency::get_antinode_positions(const std::vector<std::vector<char>> &grid, bool resonant_harmonics) const
{
    auto antinodes = find_antinode_positions_from_frequency_positions((int)grid.size(), (int)grid[0].size(), resonant_harmonics);
    return antinodes;
}

/**
 * @brief Finds antinode positions from frequency positions in the grid.
 *
 * For a given frequency, iterates through all pairs of antenna positions and calculates antinode positions.
 * An antinode occurs at any point that is in line with two antennas of the same frequency, but only when one antenna is twice as far away as the other.
 * For each pair, calculates two antinodes (one on either side), and optionally includes all positions along the line if resonant harmonics are enabled.
 *
 * @param grid_row_size Number of rows in the grid.
 * @param grid_column_size Number of columns in the grid.
 * @param resonant_harmonics Whether to consider resonant harmonics (include all positions along the line).
 * @return An unordered_set of Position objects representing antinode locations.
 */
std::unordered_set<Position> Frequency::find_antinode_positions_from_frequency_positions(const int &grid_row_size, const int &grid_column_size, bool resonant_harmonics) const
{
    std::unordered_set<Position> antinode_positions;

    // an antinode occurs at any point that is in line with two antennas of the same frequency -
    // but only when one of the antennas is twice as far away as the other.
    // This means that for any pair of antennas with the same frequency, there are two antinodes, one on either side of them.

    if (frequency_positions.size() < 2)
    {
        //std::cout << "Not enough antennas to form an antinode for frequency: " << frequency_char << std::endl;
        return antinode_positions;
    }

    // Iterate through all frequency positions to find pairs
    for (const auto &pos : frequency_positions)
    {
        // We want to check for any frequencies that are in a line with this frequency.
        for (const auto &compare_pos : frequency_positions)
        {
            if (pos == compare_pos)
                continue; // Skip self-comparison

            calculate_antinode_positions(antinode_positions, compare_pos, pos, grid_row_size, grid_column_size, resonant_harmonics);
        }
    }

    // Debug output for antinode positions
    /* std::cout << "Found " << antinode_positions.size() << " antinode positions for frequency: " << frequency_char << std::endl;
    std::cout << "Antinode positions: ";
    for (const auto &pos : antinode_positions)
    {
        std::cout << "(" << pos.x_position << ", " << pos.y_position << ") ";
    }
    std::cout << std::endl; */

    return antinode_positions;
}

/**
 * @brief Calculates antinode positions for a given pair of frequency positions.
 *
 * For a pair of antennas of the same frequency, calculates the antinode positions that are in line with both antennas,
 * and at twice the distance from one antenna to the other. If resonant harmonics is enabled, continues along the line
 * to add further antinodes and includes the antenna positions themselves as antinodes.
 *
 * @param antinode_positions Set to store resulting antinode positions.
 * @param compare_pos The position of the second antenna.
 * @param pos The position of the first antenna.
 * @param grid_row_size Number of rows in the grid.
 * @param grid_column_size Number of columns in the grid.
 * @param resonant_harmonics Whether to consider resonant harmonics (continue along the line).
 */
void Frequency::calculate_antinode_positions(std::unordered_set<Position> &antinode_positions, const Position &compare_pos, const Position &pos, const int &grid_row_size, const int &grid_column_size, bool resonant_harmonics) const
{
    // The antinode positions are twice the distance from the first position to the second
    int dx = compare_pos.x_position - pos.x_position;
    int dy = compare_pos.y_position - pos.y_position;

    int candidate_1_x_position = pos.x_position + 2 * dx;
    int candidate_1_y_position = pos.y_position + 2 * dy;
    int candidate_2_x_position = compare_pos.x_position - 2 * dx;
    int candidate_2_y_position = compare_pos.y_position - 2 * dy;

    // Check if the antinode positions are within the grid bounds
    while (candidate_1_x_position > -1 && candidate_1_x_position < grid_column_size &&
           candidate_1_y_position > -1 && candidate_1_y_position < grid_row_size)
    {
        antinode_positions.insert(Position(candidate_1_x_position, candidate_1_y_position));
        candidate_1_x_position += dx;
        candidate_1_y_position += dy;

        if (!resonant_harmonics)
        {
            break; // If resonant harmonics are not considered, we stop
        }
    }

    while (candidate_2_x_position > -1 && candidate_2_x_position < grid_column_size &&
           candidate_2_y_position > -1 && candidate_2_y_position < grid_row_size)
    {
        antinode_positions.insert(Position(candidate_2_x_position, candidate_2_y_position));
        candidate_2_x_position -= dx;
        candidate_2_y_position -= dy;
        if (!resonant_harmonics)
        {
            break; // If resonant harmonics are considered, we stop
        }
    }

    if (resonant_harmonics)
    {
        antinode_positions.insert(pos);
        antinode_positions.insert(compare_pos);
    }

    return;
}

/**
 * @brief Equality operator for Frequency.
 * @param other The other Frequency to compare.
 * @return True if frequency characters are equal.
 */
bool Frequency::operator==(const Frequency &other) const
{
    return frequency_char == other.frequency_char;
}