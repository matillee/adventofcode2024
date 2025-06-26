"""
This module handles the logic for the Guard Gallivant puzzle. It simulates the movement of a guard
on a 2D map, tracking the positions visited and handling obstacles.
"""

from typing import List, Tuple, Union


class PuzzleInput:
    """Handles reading and parsing of the puzzle input file."""

    def __init__(self, filename: str = "day6/puzzle_input"):
        """
        Initializes the PuzzleInput object and reads the file content.

        Args:
            filename (str): The path to the input file. Defaults to "day6/puzzle_input".

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


class GuardGallivant:
    """Handles the logic for the Guard Gallivant puzzle."""

    def __init__(self, puzzle_input: PuzzleInput):
        """
        Initializes the GuardGallivant object with the parsed input.

        Args:
            puzzle_input (PuzzleInput): The parsed input from the PuzzleInput class.
        """
        self.map = puzzle_input.parse_input()
        self.obstacles = self.find_obstacles()
        self.direction_map = {
                '^': [-1, 0],
                'v': [1, 0],
                '<': [0, -1],
                '>': [0, 1],
            }
        self.patrolling_map = [[cell for cell in row] for row in self.map]  # Create a copy of the map for patrolling

    def solve(self) -> int:
        """
        Solves the Guard Gallivant puzzle.

        Simulates the guard's movement on the map, tracking all distinct positions visited.

        Returns:
            int: The number of distinct positions the guard visits before leaving the map.
        """
        visited_positions = set()
        self.guard_position, self.guard_direction = self.find_guard()
        print(f"Guard starts at position {self.guard_position} facing {self.guard_direction}")
        visited_positions.add(tuple(self.guard_position))
        self.patrolling_map[self.guard_position[0]][self.guard_position[1]] = '~'  # Mark the starting position as visited
        # Simulate the guard's movement
        print("Starting guard movement...")
        while True:
            new_position, new_direction = self.move_guard(self.guard_position, self.guard_direction)
            if new_position == -1:
                break
            visited_positions.add(tuple(new_position))
            self.patrolling_map[self.guard_position[0]][self.guard_position[1]] = self.guard_direction
            self.guard_position = new_position
            self.guard_direction = new_direction
        #print(f"Guard moved to {self.guard_position} facing {self.guard_direction}")

        print("Guard has left the area.")
        self.print_map_with_patrolled_area(visited_positions)
        print(f"Total distinct positions visited: {len(visited_positions)}")
        return len(visited_positions)

    def find_guard(self) -> Tuple[List[int], str]:
        """
        Finds the guard's starting position and direction on the map.

        Returns:
            Tuple[List[int], str]: The guard's starting position and direction.
        """
        for i, row in enumerate(self.map):
            for j, cell in enumerate(row):
                if cell in "^<>v":
                    return [i, j], cell
        raise ValueError("Guard starting position not found on the map.")

    def move_guard(self, position: List[int], direction: str) -> Tuple[Union[List[int], int], str]:
        """
        Moves the guard in the specified direction.

        Args:
            position (List[int]): The current position of the guard.
            direction (str): The direction to move ('^', '<', '>', 'v').

        Returns:
            Tuple[Union[List[int], int], str]: A tuple containing:
                - The new position of the guard or -1 if the guard has left the map.
                - The new direction of the guard after turning if an obstacle is encountered.
        """
        move = self.direction_map[direction]
        new_position = [position[0] + move[0], position[1] + move[1]]

        if not self.is_valid_position(new_position):
            return -1, direction
        if self.encounter_obstacle(new_position):
            #print(f"Guard encountered an obstacle at {new_position}.")
            return position, self.turn_guard(direction)

        return new_position, direction

    def encounter_obstacle(self, position: List[int]) -> bool:
        """
        Checks if the guard encounters an obstacle at the given position.

        Args:
            position (List[int]): The position to check.

        Returns:
            bool: True if there is an obstacle, False otherwise.
        """
        return self.map[position[0]][position[1]] == '#'

    def turn_guard(self, direction: str) -> str:
        """
        Turns the guard 90 degrees clockwise.

        Args:
            direction (str): The current direction of the guard ('^', '<', '>', 'v').

        Returns:
            str: The new direction after turning.
        """
        directions = ['^', '>', 'v', '<']
        return directions[(directions.index(direction) + 1) % len(directions)]

    def is_valid_position(self, position: List[int]) -> bool:
        """
        Checks if the given position is valid within the map boundaries.

        Args:
            position (List[int]): The position to check.

        Returns:
            bool: True if the position is valid, False otherwise.
        """
        return 0 <= position[0] < len(self.map) and 0 <= position[1] < len(self.map[0])

    def find_obstacles(self) -> List[List[int]]:
        """
        Finds all obstacles in the map.

        Returns:
            List[List[int]]: A list of coordinates where obstacles are located.
        """
        return [[i, j] for i, row in enumerate(self.map) for j, cell in enumerate(row) if cell == '#']

    def print_map_with_patrolled_area(self, visited_positions: set):
        """
        Prints the map with the patrolled area marked.

        Args:
            visited_positions (set): A set of positions that have been visited by the guard.
        """
        for i, row in enumerate(self.map):
            for j, cell in enumerate(row):
                if (i, j) in visited_positions:
                    print('~', end=' ')
                else:
                    print(cell, end=' ')
            print()


if __name__ == "__main__":
    # Example usage
    puzzle_input = PuzzleInput()
    #puzzle_input = PuzzleInput(filename="day6/small_puzzle_input")
    guard_gallivant = GuardGallivant(puzzle_input)
    solution = guard_gallivant.solve()
    print(f"The solution to part one of the Guard Gallivant puzzle is: {solution}")
