"""
This module processes a word search puzzle to find occurrences of the word "XMAS" in various directions
(horizontal, vertical, diagonal) and counts specific patterns like "MAS" in an X formation.
"""

from typing import List


class PuzzleInput:
    """Handles reading and parsing of the word search puzzle input file."""

    def __init__(self, filename: str = "day4/puzzle_input"):
        """
        Initializes the PuzzleInput object and reads the file content.

        Args:
            filename (str): The path to the input file. Defaults to "day4/puzzle_input".

        Raises:
            FileNotFoundError: If the file does not exist.
            IOError: If there is an error reading the file.
        """
        self.filename = filename
        self.file_content = self._read_file()

    def _read_file(self) -> List[str]:
        """
        Reads the content of the input file.

        Returns:
            List[str]: A list of strings, each representing a line in the file.

        Raises:
            FileNotFoundError: If the file does not exist.
            IOError: If there is an error reading the file.
        """
        try:
            with open(self.filename, "r") as file:
                return [line.strip() for line in file if line.strip()]
        except FileNotFoundError:
            raise FileNotFoundError(f"Error: The file {self.filename} does not exist.")
        except IOError as e:
            raise IOError(f"Error reading file {self.filename}: {e}")

    def parse_input(self) -> List[List[str]]:
        """
        Parses the input file into a 2D list of characters.

        Returns:
            List[List[str]]: A 2D list where each inner list represents a row of characters.

        Raises:
            ValueError: If the file content is empty.
        """
        if not self.file_content:
            raise ValueError(f"No valid content in the file {self.filename}")
        return [list(line) for line in self.file_content]


class CeresSearch:
    """Handles searching for patterns in the word search puzzle."""

    def count_xmas_horizontal(self, array: List[List[str]]) -> int:
        """
        Counts occurrences of "XMAS" in horizontal rows (both forward and backward).

        Args:
            array (List[List[str]]): The 2D array representing the word search puzzle.

        Returns:
            int: The count of "XMAS" in horizontal rows.
        """
        count = 0
        for row in array:
            row_str = ''.join(row)
            count += row_str.count("XMAS")
            count += row_str[::-1].count("XMAS")
        
        print(f"Counted {count} XMAS in horizontal direction.")

        return count

    def count_xmas_vertical(self, array: List[List[str]]) -> int:
        """
        Counts occurrences of "XMAS" in vertical columns (both forward and backward).

        Args:
            array (List[List[str]]): The 2D array representing the word search puzzle.

        Returns:
            int: The count of "XMAS" in vertical columns.
        """
        count = 0
        for col in range(len(array[0])):
            column_data = ''.join(row[col] for row in array)
            count += column_data.count("XMAS")
            count += column_data[::-1].count("XMAS")

        print(f"Counted {count} XMAS in vertical direction.")

        return count

    def count_xmas_diagonal(self, array: List[List[str]]) -> int:
        """
        Counts occurrences of "XMAS" in diagonals (both directions).

        Args:
            array (List[List[str]]): The 2D array representing the word search puzzle.

        Returns:
            int: The count of "XMAS" in diagonals.
        """
        count = 0
        n = len(array)
        m = len(array[0]) if n > 0 else 0

        # Top-left to bottom-right
        for i in range(n):
            for j in range(m):
                if i + 3 < n and j + 3 < m:
                    diagonal = ''.join(array[i + k][j + k] for k in range(4))
                    count += diagonal.count("XMAS")
                    count += diagonal[::-1].count("XMAS")

        # Top-right to bottom-left
        for i in range(n):
            for j in range(m):
                if i + 3 < n and j - 3 >= 0:
                    diagonal = ''.join(array[i + k][j - k] for k in range(4))
                    count += diagonal.count("XMAS")
                    count += diagonal[::-1].count("XMAS")

        print(f"Counted {count} XMAS in diagonal direction.")

        return count

    def count_xmas_all(self, array: List[List[str]]) -> int:
        """
        Counts all occurrences of "XMAS" in the puzzle (horizontal, vertical, diagonal).

        Args:
            array (List[List[str]]): The 2D array representing the word search puzzle.

        Returns:
            int: The total count of "XMAS" in all directions.
        """
        print(f"Parsing array with {len(array)} rows and {len(array[0])} columns.")

        total_count = (
            self.count_xmas_horizontal(array)
            + self.count_xmas_vertical(array)
            + self.count_xmas_diagonal(array)
        )
        print(f"Total XMAS count: {total_count}")
        return total_count


    def count_mas_in_x_formation(self, array: List[List[str]]) -> int:
        """
        Counts occurrences of "MAS" in an X formation.

        Args:
            array (List[List[str]]): The 2D array representing the word search puzzle.

        Returns:
            int: The count of "MAS" in an X formation.
        """

        count = 0
        n = len(array)
        m = len(array[0]) if n > 0 else 0

        for i in range(1, n - 1):
            for j in range(1, m - 1):
                # Check all possible X formations
                if (
                    array[i][j] == 'A'
                    and (
                        (array[i - 1][j - 1] == 'M' and array[i - 1][j + 1] == 'M' and
                         array[i + 1][j - 1] == 'S' and array[i + 1][j + 1] == 'S')
                        or
                        (array[i - 1][j - 1] == 'S' and array[i - 1][j + 1] == 'S' and
                         array[i + 1][j - 1] == 'M' and array[i + 1][j + 1] == 'M')
                        or
                        (array[i - 1][j - 1] == 'M' and array[i - 1][j + 1] == 'S' and
                         array[i + 1][j - 1] == 'M' and array[i + 1][j + 1] == 'S')
                        or
                        (array[i - 1][j - 1] == 'S' and array[i - 1][j + 1] == 'M' and
                         array[i + 1][j - 1] == 'S' and array[i + 1][j + 1] == 'M')
                    )
                ):
                    #print(f"Found MAS at ({i}, {j}) in X formation.")
                    count += 1
        
        print(f"Counted {count} MAS in X formation.")
        return count


if __name__ == "__main__":
    # Initialize the input handler
    input_handler = PuzzleInput()

    # Parse the input into a 2D array
    puzzle_array = input_handler.parse_input()

    # Part 1: Count all occurrences of "XMAS"
    ceres_search = CeresSearch()
    print("Part 1:")
    print(ceres_search.count_xmas_all(puzzle_array))

    # Part 2: Count occurrences of "MAS" in X formation
    print("Part 2:")
    print(ceres_search.count_mas_in_x_formation(puzzle_array))