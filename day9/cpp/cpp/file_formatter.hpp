/**
 * @file file_formatter.hpp
 * @brief Declares classes and methods for disk map parsing, file and free space management, and checksum calculation.
 */
#pragma once

#include <vector>
#include <string>
#include <variant>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>

/**
 * @enum FileModification
 * @brief Represents whether something has never, partially or fully been modified.
 */
enum FileModification
{
    NEVER_MODIFIED,
    PARTIALLY_MODIFIED,
    FULLY_MODIFIED
};

/**
 * @class FreeSpace
 * @brief Represents a free space block on the disk, displayed as a dot ('.').
 * @param size The size of the free space block.
 * @param start_position The starting position of the free space block.
 * @param free_space_modification Modification status of the free space.
 */
class FreeSpace
{
public:
    FreeSpace(size_t start_position);
    FreeSpace(size_t size, size_t start_position);
    char free_space_char;
    size_t size;
    size_t start_position;
    enum FileModification free_space_modification;
    friend std::ostream &operator<<(std::ostream &os, const FreeSpace &free_space);
};

/**
 * @class FileFragment
 * @brief Represents a file fragment on the disk, identified by a file ID.
 * @param file_id The file ID for this fragment.
 * @param current_position The current position of the fragment on the disk.
 * @param fragment_modification Modification status of the fragment.
 */
class FileFragment
{
public:
    FileFragment(size_t file_id, size_t current_position);
    size_t file_id;
    size_t current_position;
    enum FileModification fragment_modification;
    friend std::ostream &operator<<(std::ostream &os, const FileFragment &file);
};

/**
 * @class File
 * @brief Represents a collection of file fragments belonging to a single file on the disk.
 * @param fragments Vector of file fragments.
 * @param fragmentation_allowed Whether fragmentation is allowed for this file.
 * @param size Number of fragments in the file.
 * @param start_position Starting position of the file.
 * @param file_modification Modification status of the file.
 * @param move_attempted Whether a move has been attempted for the file.
 */
class File
{
public:
    File() = default;
    File(std::vector<FileFragment> fragments, bool fragmentation_allowed);
    File(const File &other);
    File(File &&other) noexcept;
    File &operator=(const File &other);
    File &operator=(File &&other) noexcept;
    ~File() = default;

    size_t size;
    size_t start_position;
    std::vector<FileFragment> file_fragments;
    bool fragmentation_allowed;
    enum FileModification file_modification;
    bool move_attempted;
    bool update_file_positions(FreeSpace &free_space);
    friend std::ostream &operator<<(std::ostream &os, const File &file);

private:
    bool update_fragmented_file_positions(FreeSpace &free_space);
    bool update_entire_file_positions(FreeSpace &free_space);
    size_t update_pos(FreeSpace &free_space);
};

/**
 * @class EmptySpace
 * @brief Represents an empty space block on the disk, displayed as a dash ('-').
 * @param size The size of the empty space block.
 * @param start_position The starting position of the empty space block.
 */
class EmptySpace
{
public:
    EmptySpace(size_t start_position);
    EmptySpace(size_t size, size_t start_position);
    char empty_space_char;
    size_t size;
    size_t start_position;
    friend std::ostream &operator<<(std::ostream &os, const EmptySpace &empty_space);
};

/**
 * @class FileFormatter
 * @brief Handles parsing, formatting, and checksum calculation for disk maps.
 * @param original_diskmap The original disk map as a vector of characters.
 * @param fragmentation_allowed Whether fragmentation is allowed for files.
 * @param diskmap_size The total size of the disk map.
 * @param diskmap_free_space Vector of free space blocks.
 * @param diskmap_files Vector of files.
 * @param number_of_file_fragments Total number of file fragments.
 */
class FileFormatter
{
public:
    FileFormatter(const std::vector<char> &original_diskmap, bool fragmentation_allowed);
    size_t get_checksum();
    std::vector<std::string> print_diskmap_order();

private:
    size_t number_of_file_fragments;
    std::vector<char> original_diskmap;
    bool fragmentation_allowed;
    size_t convert_char_to_file_id(char c);
    std::string convert_file_id_to_str(size_t file_id);
    size_t diskmap_size;
    std::vector<FreeSpace> diskmap_free_space;
    std::vector<File> diskmap_files;
    void parse_original_diskmap(const std::vector<char> &original_diskmap, bool fragmentation_allowed);
    void format_diskmap();
    void format_diskmap_no_fragmentation();
    size_t calculate_checksum();
    void print_diskmap_information();
    void print_file_information(const File &file);
    void find_free_space(File &file);
    void swap_free_space_with_file(FreeSpace &free_space);
    void swap_free_space_with_file(FreeSpace &free_space, std::vector<File>::iterator &backwards_it);
};

/**
 * @class ManagerClass
 * @brief Handles reading the disk map from file and providing the checksum interface.
 * @param input_file_name The path to the input file.
 * @param original_diskmap The original disk map as a vector of characters.
 */
class ManagerClass
{
public:
    ManagerClass(const std::string &input_file_name);
    size_t get_checksum();
    size_t get_checksum_for_whole_files();

private:
    std::vector<char> original_diskmap;
    std::vector<char> read_input(const std::string &filename);
};