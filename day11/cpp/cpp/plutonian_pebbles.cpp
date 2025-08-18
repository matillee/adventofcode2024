#include "plutonian_pebbles.hpp"

PlutonianPebbleTransformer::PlutonianPebbleTransformer(const std::vector<size_t> &starting_pebble_order)
    : current_pebble_order(convert_integers_to_pebbles(std::move(starting_pebble_order))) {};

std::vector<PlutonianPebble> PlutonianPebbleTransformer::convert_integers_to_pebbles(const std::vector<size_t> &pebble_order)
{
    std::vector<PlutonianPebble> pebbles;
    for (auto engraved_number : pebble_order)
    {
        pebbles.emplace_back(PlutonianPebble(engraved_number));
    }
    return pebbles;
}

void PlutonianPebbleTransformer::print_pebbles()
{
    for (const auto &pebble : current_pebble_order)
    {
        std::cout << pebble;
    }
    std::cout << std::endl;
}

size_t PlutonianPebbleTransformer::get_number_of_pebbles_after_blinking(size_t number_of_blinks)
{

    while (number_of_blinks--)
    {
        // print_pebbles();
        std::vector<std::pair<size_t, PlutonianPebble>> pebbles_to_insert;
        for (size_t idx = 0; idx < current_pebble_order.size(); ++idx)
        {
            auto &pebble = current_pebble_order[idx];
            auto result = pebble.apply_plutonian_pebble_rule();
            if (result)
            {
                // Collect insertion info: insert before current pebble
                pebbles_to_insert.emplace_back(idx, *result);
            }
        }
        // Insert new pebbles after iteration to avoid invalidating iterators
        // Offset for each insertion since vector grows
        size_t offset = 0;
        for (const auto &pair : pebbles_to_insert)
        {
            current_pebble_order.insert(current_pebble_order.begin() + pair.first + offset, pair.second);
            ++offset;
        }
    }
    return current_pebble_order.size();
    size_t sum = 0;
    for (auto &pebble : current_pebble_order)
    {
        sum += pebble.engraved_number;
    }
    return sum;
}

PlutonianPebble::PlutonianPebble(size_t engraved_number) : engraved_number(engraved_number) {};

/**
 * @brief Output stream operator for PlutonianPebble.
 * @param os The output stream.
 * @param free_space The FreeSpace to print.
 * @return The output stream.
 */
std::ostream &operator<<(std::ostream &os, const PlutonianPebble &pebble)
{
    os << "(" + std::to_string(pebble.engraved_number) + ")";

    return os;
}

std::optional<PlutonianPebble> PlutonianPebble::apply_plutonian_pebble_rule()
{
    // std::cout << "Apply rule on number " << engraved_number << std::endl;
    if (engraved_number == 0)
    {
        engraved_number = 1;
        return std::nullopt;
    }

    auto number_of_digits = std::to_string(engraved_number).size();

    if (number_of_digits % 2 == 0)
    {
        auto separated_numbers = get_separated_numbers(number_of_digits);
        engraved_number = separated_numbers.second;
        return PlutonianPebble(separated_numbers.first);
    }
    engraved_number *= 2024;
    return std::nullopt;
}

std::pair<size_t, size_t> PlutonianPebble::get_separated_numbers(size_t number_of_digits)
{

    // Update this pebble with last digits of engraved_number and create new pebble with the first digits.
    std::pair<size_t, size_t> number_pair;
    // std::cout << "Get separated numbers for " << engraved_number << std::endl;

    number_pair.first = convert_to_integer(std::to_string(engraved_number).substr(0, number_of_digits / 2));

    auto new_pebble_number_2 = std::to_string(engraved_number).substr(number_of_digits / 2, number_of_digits / 2);
    if (new_pebble_number_2.length() == 1)
    {
        number_pair.second = convert_to_integer(new_pebble_number_2);
        return number_pair;
    }

    // Remove all zeros at the start of the number
    number_pair.second = remove_leading_zeros(new_pebble_number_2);
    /* for (size_t index = 0; index < new_pebble_number_2.length(); index++)
    {
        if (new_pebble_number_2[index] != '0')
        {
            auto substring_test = new_pebble_number_2.substr(index);
            number_pair.second = convert_to_integer(substring_test);
            break;
        }
    }
    if (!number_pair.second)
        number_pair.second = 0; */

    return number_pair;
}

size_t PlutonianPebble::convert_to_integer(const std::string &str)
{
    return stoull(str);
    // std::cout << "Before converting to int " << str << std::endl;
    size_t my_new_int;
    std::stringstream tmp_stream(str);
    tmp_stream >> my_new_int;
    // std::cout << "After converting to int " << my_new_int << std::endl;
    return my_new_int;
}

size_t PlutonianPebble::remove_leading_zeros(const std::string &str)
{
    size_t first_non_zero = str.find_first_not_of('0');
    if (first_non_zero == std::string::npos)
    {
        // All characters are '0'
        return 0;
    }
    return convert_to_integer(str.substr(first_non_zero));
}
