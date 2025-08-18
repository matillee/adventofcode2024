/**
 * @file plutonian_pebbles.hpp
 * @brief Declares classes and methods for pebbles analysis-
 */
#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <variant>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <optional>

class PlutonianPebble
{
public:
    PlutonianPebble(size_t engraved_number);
    std::optional<PlutonianPebble> apply_plutonian_pebble_rule();
    size_t engraved_number;
    friend std::ostream &operator<<(std::ostream &os, const PlutonianPebble &pebble);
private:
    std::pair<size_t, size_t> get_separated_numbers(size_t number_of_digits);
    static size_t convert_to_integer(const std::string &str);
    static size_t remove_leading_zeros(const std::string &str);
};

class PlutonianPebbleTransformer
{
public:
    PlutonianPebbleTransformer(const std::vector<size_t> &starting_pebble_order);
    size_t get_number_of_pebbles_after_blinking(size_t number_of_blinks);

private:
    std::vector<PlutonianPebble> current_pebble_order;
    size_t number_of_blinks;
    std::vector<PlutonianPebble> convert_integers_to_pebbles(const std::vector<size_t> &pebble_order);
    void print_pebbles();
};

/**
 * @class ManagerClass
 * @brief Handles reading the pebble order from file and providing the pebble changing interface.
 * @param input_file_name The path to the input file.
 */
class ManagerClass
{
public:
    ManagerClass(const std::string &input_file_name);
    size_t get_number_of_pebbles(size_t number_of_blinks);

private:
    std::vector<size_t> starting_pebble_order;
    std::vector<size_t> read_input(const std::string &filename);
};