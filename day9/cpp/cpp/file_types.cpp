/**
 * @file file_types.cpp
 * @brief Implements classes and methods for file fragments, free space, empty space, and file management on a disk map.
 */
#include "file_formatter.hpp"

/**
 * @brief Constructs a FreeSpace object with a single block.
 * @param start_position The starting position of the free space block.
 */
FreeSpace::FreeSpace(size_t start_position)
    : free_space_char('.'),
      size(1),
      start_position(start_position),
      free_space_modification(FileModification::NEVER_MODIFIED) {};

/**
 * @brief Constructs a FreeSpace object with a specified size.
 * @param size The size of the free space block.
 * @param start_position The starting position of the free space block.
 */
FreeSpace::FreeSpace(size_t size, size_t start_position)
    : free_space_char('.'),
      size(size),
      start_position(start_position),
      free_space_modification(FileModification::NEVER_MODIFIED) {};

/**
 * @brief Output stream operator for FreeSpace.
 * @param os The output stream.
 * @param free_space The FreeSpace to print.
 * @return The output stream.
 */
std::ostream &operator<<(std::ostream &os, const FreeSpace &free_space)
{
    for (size_t i = 0; i < free_space.size; ++i)
    {
        os << free_space.free_space_char;
    }
    return os;
}

/**
 * @brief Constructs an EmptySpace object with a single block.
 * @param start_position The starting position of the empty space block.
 */
EmptySpace::EmptySpace(size_t start_position)
    : empty_space_char('-'), size(1), start_position(start_position) {};

/**
 * @brief Constructs an EmptySpace object with a specified size.
 * @param size The size of the empty space block.
 * @param start_position The starting position of the empty space block.
 */
EmptySpace::EmptySpace(size_t size, size_t start_position)
    : empty_space_char('-'), size(size), start_position(start_position) {};

/**
 * @brief Output stream operator for EmptySpace.
 * @param os The output stream.
 * @param empty_space The EmptySpace to print.
 * @return The output stream.
 */
std::ostream &operator<<(std::ostream &os, const EmptySpace &empty_space)
{
    for (size_t i = 0; i < empty_space.size; ++i)
    {
        os << empty_space.empty_space_char;
    }
    return os;
}

/**
 * @brief Constructs a FileFragment object.
 * @param file_id The file ID for this fragment.
 * @param current_position The current position of the fragment on the disk.
 */
FileFragment::FileFragment(size_t file_id, size_t current_position)
    : file_id(file_id),
      current_position(current_position),
      fragment_modification(NEVER_MODIFIED) {};

/**
 * @brief Output stream operator for FileFragment.
 * @param os The output stream.
 * @param file_fragment The FileFragment to print.
 * @return The output stream.
 */
std::ostream &operator<<(std::ostream &os, const FileFragment &file_fragment)
{
    os << file_fragment.file_id;
    return os;
}

/**
 * @brief Constructs a File object from fragments and fragmentation flag.
 * @param fragments Vector of file fragments.
 * @param fragmentation_allowed Whether fragmentation is allowed for this file.
 */
File::File(std::vector<FileFragment> fragments, bool fragmentation_allowed)
    : size(fragments.size()),
      start_position(fragments[0].current_position),
      file_fragments(std::move(fragments)),
      fragmentation_allowed(fragmentation_allowed),
      file_modification(FileModification::NEVER_MODIFIED),
      move_attempted(false) {};

/**
 * @brief Copy constructor for File.
 * @param other The File object to copy from.
 */
File::File(const File &other)
    : size(other.size),
      start_position(other.start_position),
      file_fragments(other.file_fragments),
      fragmentation_allowed(other.fragmentation_allowed),
      file_modification(other.file_modification) {};

/**
 * @brief Move constructor for File.
 * @param other The File object to move from.
 */
File::File(File &&other) noexcept
    : size(other.size),
      start_position(other.start_position),
      file_fragments(std::move(other.file_fragments)),
      fragmentation_allowed(other.fragmentation_allowed),
      file_modification(other.file_modification) {};

/**
 * @brief Copy assignment operator for File.
 * @param other The File object to copy from.
 * @return Reference to this File object.
 */
File &File::operator=(const File &other)
{
    if (this != &other)
    {
        size = other.size;
        start_position = other.start_position;
        file_fragments = other.file_fragments;
        fragmentation_allowed = other.fragmentation_allowed;
        file_modification = other.file_modification;
    }
    return *this;
};

/**
 * @brief Move assignment operator for File.
 * @param other The File object to move from.
 * @return Reference to this File object.
 */
File &File::operator=(File &&other) noexcept
{
    if (this != &other)
    {
        size = other.size;
        start_position = other.start_position;
        file_fragments = std::move(other.file_fragments);
        fragmentation_allowed = other.fragmentation_allowed;
        file_modification = other.file_modification;
    }
    return *this;
};

/**
 * @brief Updates file fragment positions using available free space.
 * @param free_space The free space block to use for moving fragments.
 * @return True if any fragments were moved, false otherwise.
 */
bool File::update_file_positions(FreeSpace &free_space)
{

    if (this->file_modification == FileModification::FULLY_MODIFIED)
        return false;

    if (this->fragmentation_allowed)
    {
        return update_fragmented_file_positions(free_space);
    }
    else
    {
        /* if(move_attempted){
            return false;
        } */
        return update_entire_file_positions(free_space);
    }
}

/**
 * @brief Updates fragmented file positions using available free space.
 * @param free_space The free space block to use for moving fragments.
 * @return True if any fragments were moved, false otherwise.
 */
bool File::update_fragmented_file_positions(FreeSpace &free_space)
{
    auto counter = update_pos(free_space);

    bool all_fragments_modified = std::all_of(this->file_fragments.begin(), this->file_fragments.end(),
                                              [first = this->file_fragments[0].fragment_modification](const FileFragment &fragment)
                                              {
                                                  return fragment.fragment_modification == FileModification::FULLY_MODIFIED;
                                              });

    if (all_fragments_modified)
    {
        this->file_modification = FileModification::FULLY_MODIFIED;
    }
    else
    {
        this->file_modification = FileModification::PARTIALLY_MODIFIED;
    }
    if (counter > 0)
        return true;
    else
        return false;
}

/**
 * @brief Updates entire file positions if free space is large enough.
 * @param free_space The free space block to use for moving the file.
 * @return True if the file was moved, false otherwise.
 */
bool File::update_entire_file_positions(FreeSpace &free_space)
{
    this->move_attempted = true;
    if (free_space.size >= this->size)
    {
        // std::cout << "FreeSpace size larger than number of file fragments, we can add this to the free space." << std::endl;
        update_pos(free_space);
        this->file_modification = FileModification::FULLY_MODIFIED;
        return true;
    }
    else
    {
        // std::cout << "FreeSpace size smaller than number of file fragments, cannot use file fragments for " << this->file_fragments[0] <<"." << std::endl;
        return false;
    }
}

/**
 * @brief Updates fragment positions and modifies free space accordingly.
 * @param free_space The free space block to use for moving fragments.
 * @return Number of fragments moved.
 */
size_t File::update_pos(FreeSpace &free_space)
{
    int search_index = this->file_fragments.size() - 1;
    size_t counter = 0;
    bool found_replacement = false;
    for (size_t available_pos = free_space.start_position; available_pos < free_space.start_position + free_space.size; ++available_pos)
    {
        found_replacement = false;
        while (!found_replacement)
        {
            if (search_index < 0)
                break;

            if (this->file_fragments[search_index].fragment_modification != FileModification::FULLY_MODIFIED)
            {
                this->file_fragments[search_index].current_position = available_pos;
                this->file_fragments[search_index].fragment_modification = FileModification::FULLY_MODIFIED;
                ++counter;
                found_replacement = true;
            }
            --search_index;
        }
    }

    free_space.size -= counter;
    free_space.start_position += counter;
    return counter;
}

/**
 * @brief Output stream operator for File.
 * @param os The output stream.
 * @param file The File to print.
 * @return The output stream.
 */
std::ostream &operator<<(std::ostream &os, const File &file)
{
    for (const auto &fragment : file.file_fragments)
    {
        os << fragment;
    }
    return os;
}
