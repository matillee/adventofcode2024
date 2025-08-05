#include "resonant_collinearity.hpp"

/**
 * @brief Entry point. Runs the simulation and prints the results for part one and part two.
 */
int main(int argc, char *argv[])
{
    std::cout << "Resonant Collinearity Simulation\n";
    std::cout << "==========================\n";
    std::string filename;
    if (argc > 2)
    {
        std::cout << "Filename provided: " << argv[1] << " \n";
        filename = argv[1];
    }
    else if (argc < 2)
    {
        //filename = "../puzzle_input";
        filename = "../small_puzzle_input";
        // filename = "../t_puzzle_input";
        std::cout << "No input file provided. Using default filename " << filename << "\n";
    }

    auto unique_antinode_positions = ManagerClass(filename).get_unique_antinode_positions();
    std::cout << "Part One: " << unique_antinode_positions.size() << "\n";

    auto unique_antinode_positions_with_resonant_harmonics = ManagerClass(filename).get_unique_antinode_positions_with_resonant_harmonics();
    std::cout << "Part Two: " << unique_antinode_positions_with_resonant_harmonics.size() << "\n";
}