#include "garden_groups.hpp"

/**
 * @brief Entry point. Runs the simulation and prints the results for part one and part two.
 */
int main(int argc, char *argv[])
{
    std::cout << "Garden Groups Simulation\n";
    std::cout << "==========================\n";
    std::string filename;
    if (argc > 2)
    {
        std::cout << "Filename provided: " << argv[1] << " \n";
        filename = argv[1];
    }
    else if (argc < 2)
    {
        filename = "../puzzle_input";
        filename = "../small_puzzle_input";
        std::cout << "No input file provided. Using default filename " << filename << "\n";
    }

    auto score = ManagerClass(filename).get_fence_pricing();
    std::cout << "Part One: " << score << std::endl << std::endl << std::endl;

    //std::cout << "Part Two: " << sum << "\n";
}