"""
This module processes corrupted memory instructions to extract valid multiplication operations
and compute their results. It handles enabling/disabling of instructions based on specific keywords.
"""

from typing import List
import re


class PuzzleInput:
    """
    Handles reading and parsing of the corrupted memory input file.
    """

    def __init__(self, filename: str = "day3/puzzle_input"):
        """
        Initialize the PuzzleInput object and read the file content.

        The input file is expected to contain corrupted memory instructions.
        If the file is empty or cannot be read, an exception is raised.

        Args:
            filename (str): The path to the input file. Defaults to "day3/puzzle_input".

        Raises:
            ValueError: If the file is empty or cannot be read.
        """
        self.filename = filename
        self.file_content: List[str] = self.__read_file()
        if not self.file_content:
            raise ValueError(f"No content in the file {self.filename}")

    def __read_file(self) -> List[str]:
        """
        Read the content of the input file.

        Returns:
            List[str]: A list of strings, each representing a line in the file.

        Raises:
            FileNotFoundError: If the file does not exist.
            IOError: If there is an error reading the file.
        """
        try:
            with open(self.filename, "r") as file:
                return file.readlines()
        except FileNotFoundError:
            raise FileNotFoundError(f"Error: The file {self.filename} does not exist.")
        except IOError as e:
            raise IOError(f"Error reading file {self.filename}: {e}")

    def parse_input(self) -> List[str]:
        """
        Extract all valid `mul(X,Y)` instructions from the file content.

        Returns:
            List[str]: A list of valid `mul(X,Y)` instructions.
        """
        regex = r"mul\((?<!\d)\d{1,3}(?!\d),(?<!\d)\d{1,3}(?!\d)\)"
        mul_list = []

        for line in self.file_content:
            mul_list += re.findall(regex, line)

        return mul_list

    def parse_input_do_and_dont(self) -> List[str]:
        """
        Extract valid `mul(X,Y)` instructions, considering `do()` and `don't()` instructions.

        - At the start, all `mul` instructions are enabled.
        - A `don't()` instruction disables all subsequent `mul` instructions until a `do()` is encountered.
        - A `do()` instruction re-enables `mul` instructions.

        Returns:
            List[str]: A list of valid `mul(X,Y)` instructions based on the `do()` and `don't()` logic.
        """
        regex = r"mul\((?<!\d)\d{1,3}(?!\d),(?<!\d)\d{1,3}(?!\d)\)"
        all_lines = "".join(self.file_content)  # Join all lines for easier processing
        return_list = []

        dont_separated_list = all_lines.split("don't")

        # Process the first segment (before any "don't")
        return_list += re.findall(regex, dont_separated_list.pop(0))

        # Process subsequent segments
        for dont_segment in dont_separated_list:
            if "do" not in dont_segment:
                # Skip this segment as all `mul` instructions are disabled
                continue

            # Split on the first "do" and process the enabled part
            _, enabled_segment = dont_segment.split("do", 1)
            return_list += re.findall(regex, enabled_segment)

        return return_list


class Translation:
    """
    Handles the computation of results from extracted `mul(X,Y)` instructions.
    """

    @staticmethod
    def summarize_multiplications(mul_list: List[str]) -> int:
        """
        Compute the sum of all multiplication results from a list of `mul(X,Y)` instructions.

        Args:
            mul_list (List[str]): A list of valid `mul(X,Y)` instructions.

        Returns:
            int: The sum of all multiplication results.

        Raises:
            ValueError: If an instruction does not contain exactly two numbers.
        """
        total_sum = 0

        for instruction in mul_list:
            numbers = re.findall(r"\d+", instruction)
            if len(numbers) != 2:
                raise ValueError(f"Unexpected values in instruction: {instruction}")

            try:
                multiplication_result = int(numbers[0]) * int(numbers[1])
                total_sum += multiplication_result
            except ValueError as e:
                raise ValueError(f"Error converting numbers in {instruction} to integers: {e}")

        print(total_sum)
        return total_sum


if __name__ == "__main__":
    # Initialize the input handler
    input_handler = PuzzleInput()

    # Part 1: Parse and compute results for all valid `mul` instructions
    part_one_parsing = input_handler.parse_input()
    print("Part 1 Result:")
    Translation.summarize_multiplications(part_one_parsing)

    # Part 2: Parse and compute results considering `do()` and `don't()` logic
    part_two_parsing = input_handler.parse_input_do_and_dont()
    print("Part 2 Result:")
    Translation.summarize_multiplications(part_two_parsing)
