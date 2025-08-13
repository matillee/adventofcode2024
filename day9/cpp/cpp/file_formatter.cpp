/**
 * @file file_formatter.cpp
 * @brief Implements disk map parsing, file and free space management, and checksum calculation logic.
 */
#include "file_formatter.hpp"

/**
 * @brief Constructs a FileFormatter object and parses the original disk map.
 * @param original_diskmap The original disk map as a vector of characters.
 * @param fragmentation_allowed Whether fragmentation is allowed for files.
 */
FileFormatter::FileFormatter(const std::vector<char> &original_diskmap, bool fragmentation_allowed)
    : original_diskmap(original_diskmap), fragmentation_allowed(fragmentation_allowed)
{
    parse_original_diskmap(original_diskmap, fragmentation_allowed);
    //std::cout << "Diskmap size: " << diskmap_size << std::endl;
}

/**
 * @brief Converts a character to its corresponding file ID (integer value).
 * @param c The character to convert (ASCII digit).
 * @return The integer file ID.
 */
size_t FileFormatter::convert_char_to_file_id(char c)
{
    return (size_t)c - 48;
}

/**
 * @brief Converts a file ID to its corresponding string.
 * @param file_id The size_t file ID.
 * @return The string representing the file ID, wrapped inside () to account for multiple digits.
 */
std::string FileFormatter::convert_file_id_to_str(size_t file_id){
    return "(" + std::to_string(file_id) + ")";
}

/**
 * @brief Parses the original disk map and populates file and free space vectors.
 * @param original_diskmap The original disk map as a vector of characters.
 * @param fragmentation_allowed Whether fragmentation is allowed for files.
 * 
 * The number in the original diskmap represents number of file fragments or free space size.
 * Even indices represent file blocks and odd indices represent free space.
 */
void FileFormatter::parse_original_diskmap(const std::vector<char> &original_diskmap, bool fragmentation_allowed)
{   
    diskmap_size = 0;
    number_of_file_fragments = 0;
    for (size_t i = 0; i < original_diskmap.size(); ++i)
    {
        if (i % 2 == 0)
        {
            std::vector<FileFragment> file_fragments;
            for (size_t j = 0; j < convert_char_to_file_id(original_diskmap[i]); ++j)
            {
                // The file ID (i/2) is based on the order of the files in the original diskmap.
                file_fragments.push_back(FileFragment(i / 2, diskmap_size));
                diskmap_size++;
                number_of_file_fragments++;
            }
            diskmap_files.push_back(File(file_fragments, fragmentation_allowed));
        }
        else
        {
            auto free_space_size = convert_char_to_file_id(original_diskmap[i]);
            if (free_space_size > 0)
            {
                diskmap_free_space.push_back(FreeSpace(free_space_size, diskmap_size));
                diskmap_size += free_space_size;
            }
        }
        continue;
    }
}

/**
 * @brief Calculates the checksum of the disk map after formatting.
 * @return The calculated checksum value.
 */
size_t FileFormatter::get_checksum()
{
    // std::cout << "Pre Formatting ";
    // print_diskmap_order();
    if (fragmentation_allowed)
    {
        format_diskmap();
    }
    else
    {
        format_diskmap_no_fragmentation();
    }

    // std::cout << "Post Formatting ";
    print_diskmap_order();
    return calculate_checksum();
}

/**
 * @brief Prints the current disk map order as a vector of characters.
 * @return A vector of characters representing the disk map order.
 */
std::vector<std::string> FileFormatter::print_diskmap_order()
{
    std::vector<std::string> printer(diskmap_size, ".");
    for (const auto &v : diskmap_files)
    {
        for (auto &fragment : v.file_fragments)
        {
            printer.at(fragment.current_position) = convert_file_id_to_str(fragment.file_id);
        }
    }
    std::cout << "Diskmap Order: " << std::endl;
    for (auto &value : printer)
    {
        std::cout << value;
    }

    std::cout << std::endl;
    return printer;
}

/**
 * @brief Prints information about all files in the disk map.
 */
void FileFormatter::print_diskmap_information()
{
    for (const auto &v : diskmap_files)
    {
        print_file_information(v);
    }
}

/**
 * @brief Prints detailed information about a single file and its fragments.
 * @param file The file to print information for.
 */
void FileFormatter::print_file_information(const File &file)
{
    std::cout << "File(size=" << file.size << ", start_position=" << file.start_position << ", modification=" << file.file_modification << ", fragmentation_allowed=" << file.fragmentation_allowed << ")" << std::endl;

    for (auto &fragment : file.file_fragments)
    {
        std::cout << "Fragment " << fragment << " with position " << fragment.current_position << std::endl;
    }
}

/**
 * @brief Calculates the checksum by summing position multiplied by file_id for all fragments.
 * @return The calculated checksum value.
 */
size_t FileFormatter::calculate_checksum()
{
    size_t checksum = 0;
    for (auto &obj : diskmap_files)
    {
        for (auto &fragment : obj.file_fragments)
        {
            checksum += (fragment.current_position * fragment.file_id);
        }
    }
    return checksum;
}

/**
 * @brief Formats the disk map by moving file fragments into free space (fragmentation allowed).
 */
void FileFormatter::format_diskmap()
{
    for (auto forwards_it = diskmap_free_space.begin(); forwards_it < diskmap_free_space.end(); ++forwards_it)
    {
        FreeSpace &free_space = *forwards_it;
        if (free_space.start_position < number_of_file_fragments)
        {
            swap_free_space_with_file(free_space);
        }
    }
    return;
}

/**
 * @brief Swaps free space with file fragments, filling free space as much as possible.
 * @param free_space The free space block to fill.
 */
void FileFormatter::swap_free_space_with_file(FreeSpace &free_space)
{
    for (auto backwards_it = diskmap_files.end() - 1; backwards_it >= diskmap_files.begin(); --backwards_it)
    {
        File &file = *backwards_it;
        if (file.update_file_positions(free_space))
        {
            //  The file fragments were used to fill some or all of the free space.
            if (free_space.size > 0)
            {
                // std::cout << "Free space not filled" << std::endl;
                //  Start again and set the iterator to the end of the parsed diskmap
                backwards_it = diskmap_files.end();
                // TODO: Probably not necessary to do this in this implementation 
                // since method is only used with fragmentation allowed.
            }
            else
            {
                // std::cout << "Free space filled" << std::endl;
                //  All free space was filled by the contents of that file
                break;
            }
        }
        // else
        // std::cout << "The file could not be used to fill the free space for some reason, continue to next one." << std::endl;
    }
    return;
}

/**
 * @brief Formats the disk map by moving whole files into free space (no fragmentation).
 */
void FileFormatter::format_diskmap_no_fragmentation()
{
    for (auto backwards_it = diskmap_files.end() - 1; backwards_it >= diskmap_files.begin(); --backwards_it)
    {
        File &file = *backwards_it;
        find_free_space(file);
    }
}

/**
 * @brief Finds free space for a given file and attempts to move it.
 * @param file The file to move into free space.
 */
void FileFormatter::find_free_space(File &file)
{
    for (auto forwards_it = diskmap_free_space.begin(); forwards_it < diskmap_free_space.end(); ++forwards_it)
    {
        FreeSpace &free_space = *forwards_it;
        if (free_space.start_position > file.start_position)
            return;
        if (file.update_file_positions(free_space))
            return;
    }
}

/**
 * @brief Recursively swaps free space with file fragments using an iterator.
 * @param free_space The free space block to fill.
 * @param backwards_it Iterator to the current file in the diskmap_files vector.
 */
void FileFormatter::swap_free_space_with_file(FreeSpace &free_space, std::vector<File>::iterator &backwards_it)
{
    // TODO: Remove this unless it can be used in other implementation (not for advent of code)
    File &file = *backwards_it;
    if (file.update_file_positions(free_space))
    {
        if (free_space.size > 0)
        {
            std::cout << "Free space not filled" << std::endl;
            swap_free_space_with_file(free_space, backwards_it);
        }
        //--backwards_it;
    }
    else
    {
        --backwards_it;
        swap_free_space_with_file(free_space, backwards_it);
    }
    return;
}
