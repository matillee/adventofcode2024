#include "guard_gallivant.hpp"

/**
 * @brief Entry point. Runs the simulation and prints the results for part one and part two.
 */
int main(int argc, char *argv[])
{
    std::cout << "Guard Gallivant Simulation\n";
    std::cout << "==========================\n";
    std::string filename;
    if (argc > 2)
    {
        std::cout << "Filename provided: " << argv[1] << " \n";
        filename = argv[1];
    }
    else if (argc < 2)
    {
        // filename = "../puzzle_input";
        filename = "../small_puzzle_input";
        std::cout << "No input file provided. Using default filename " << filename << "\n";
    }

    std::cout << "Part One: " << ManagerClass(filename).get_number_of_patrolled_positions() << "\n";

    std::cout << "Part Two: " << ManagerClass(filename).get_number_of_obstructions_for_guard_loops() << "\n";
}