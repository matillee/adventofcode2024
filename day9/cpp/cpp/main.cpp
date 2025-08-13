#include "file_formatter.hpp"

/**
 * @brief Entry point. Runs the simulation and prints the results for part one and part two.
 */
int main(int argc, char *argv[])
{
    std::cout << "Disk Fragmenter Simulation\n";
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

    auto checksum = ManagerClass(filename).get_checksum();
    std::cout << "Part One: " << checksum << std::endl << std::endl << std::endl;

    auto checksum_two = ManagerClass(filename).get_checksum_for_whole_files();
    std::cout << "Part Two: " << checksum_two << "\n";
}