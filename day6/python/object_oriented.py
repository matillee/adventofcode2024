import logging
from typing import List, Tuple
from concurrent.futures import ThreadPoolExecutor
from copy import deepcopy

# Configure logging
logging.basicConfig(
    level=logging.INFO, format="%(asctime)s - %(levelname)s - %(message)s"
)


class Position:
    """Represents a position in the Guard Gallivant puzzle."""

    def __init__(self, x: int, y: int):
        """
        Initializes the Position object.

        Args:
            x (int): The x-coordinate.
            y (int): The y-coordinate.
        """
        self.x = x
        self.y = y

    def __eq__(self, other):
        """
        Checks if two Position objects are equal.

        Args:
            other (Position): The other Position object to compare.

        Returns:
            bool: True if the positions are equal, False otherwise.
        """
        return isinstance(other, Position) and self.x == other.x and self.y == other.y

    def __hash__(self):
        """
        Returns a hash value for the Position object.

        Returns:
            int: The hash value of the Position object.
        """
        return hash((self.x, self.y))

    def __repr__(self):
        """
        Returns a string representation of the Position object.

        Returns:
            str: The string representation of the Position object.
        """
        return f"Position(x={self.x}, y={self.y})"


class Coordinate:
    """Represents a coordinate in the Guard Gallivant puzzle."""

    def __init__(self, position: Position, contains_obstacle: bool = False):
        """
        Initializes the Coordinate object.

        Args:
            position (Position): The position of the coordinate.
            contains_obstacle (bool): Whether the coordinate contains an obstacle. Defaults to False.
        """
        self.position = position
        self.contains_obstacle = contains_obstacle
        self.number_of_visits_in_direction = {
            "^": 0,
            "v": 0,
            "<": 0,
            ">": 0,
        }

    def __str__(self):
        """
        Returns a string representation of the Coordinate object.

        Returns:
            str: "#" if it contains an obstacle, "~" if visited, or "." otherwise.
        """
        if self.contains_obstacle:
            return "#"
        elif self.has_been_visited_in_direction(
            "^"
        ) or self.has_been_visited_in_direction("v"):
            if self.has_been_visited_in_direction(
                "<"
            ) or self.has_been_visited_in_direction(">"):
                return "+"
            return "|"
        elif self.has_been_visited_in_direction(
            "<"
        ) or self.has_been_visited_in_direction(">"):
            return "-"
        elif self.has_been_visited():
            return "~"
        else:
            return ";"

    def __repr__(self):
        """
        Returns a string representation of the Coordinate object.

        Returns:
            str: The string representation of the Coordinate object.
        """
        return f"Coordinate(position={self.position}, contains_obstacle={self.contains_obstacle})"

    def add_obstacle(self):
        """
        Marks the coordinate as containing an obstacle.
        """
        self.contains_obstacle = True
        for direction in self.number_of_visits_in_direction:
            self.number_of_visits_in_direction[direction] = 0

    def remove_obstacle(self):
        """
        Marks the coordinate as not containing an obstacle.
        """
        self.contains_obstacle = False

    def visit(self, direction: str):
        """
        Marks the coordinate as visited in the specified direction.

        Args:
            direction (str): The direction to move ('^', 'v', '<', '>').
        """
        self.number_of_visits_in_direction[direction] += 1

    def reset_visits(self):
        """
        Resets the visit counts for all directions.
        """
        for direction in self.number_of_visits_in_direction:
            self.number_of_visits_in_direction[direction] = 0

    def get_visits_in_direction(self, direction: str) -> int:
        """
        Gets the number of visits in the specified direction.

        Args:
            direction (str): The direction to check ('^', 'v', '<', '>').

        Returns:
            int: The number of visits in the specified direction.
        """
        return self.number_of_visits_in_direction.get(direction, 0)

    def has_been_visited_in_direction(self, direction: str) -> bool:
        """
        Checks if the coordinate had been visited in the specified direction before this turn.

        Args:
            direction (str): The direction to check ('^', 'v', '<', '>').

        Returns:
            bool: True if the coordinate was visited in the specified direction before, False otherwise.
        """
        return self.number_of_visits_in_direction.get(direction, 0) > 0

    def has_been_visited_in_direction_before(self, direction: str) -> bool:
        """
        Checks if the coordinate had been visited in the specified direction before this turn.

        Args:
            direction (str): The direction to check ('^', 'v', '<', '>').

        Returns:
            bool: True if the coordinate was visited in the specified direction before, False otherwise.
        """
        return self.number_of_visits_in_direction.get(direction, 0) > 1

    def has_been_visited(self) -> bool:
        """
        Checks if the coordinate has been visited in any direction.

        Returns:
            bool: True if the coordinate has been visited in any direction, False otherwise.
        """
        return any(visits > 0 for visits in self.number_of_visits_in_direction.values())


class CoordinateMapping:
    """Represents the map of coordinates in the Guard Gallivant puzzle."""

    def __init__(
        self,
        input_data: List[List[str]],
    ):
        """
        Initializes the CoordinateMapping object.

        Args:
            input_data (List[List[str]]): The 2D list of characters representing the map.
        """
        self._coordinate_mapping = self._create_mapping(input_data)

    def _create_mapping(self, input_data: List[List[str]]) -> List[List[Coordinate]]:
        """
        Creates the 2D list of Coordinate objects.

        Args:
            input_data (List[List[str]]): The 2D list of characters representing the map.

        Returns:
            List[List[Coordinate]]: The 2D list of Coordinate objects.
        """
        return [
            [
                self._create_coordinate(Position(x, y), cell == "#")
                for x, cell in enumerate(row)
            ]
            for y, row in enumerate(input_data)
        ]

    def _create_coordinate(
        self, position: Position, contains_obstacle: bool = False
    ) -> Coordinate:
        """
        Creates a Coordinate object for the given position.

        Args:
            position (Position): The position of the coordinate.
            contains_obstacle (bool): Whether the coordinate contains an obstacle. Defaults to False.

        Returns:
            Coordinate: A Coordinate object initialized with the given parameters.
        """
        return Coordinate(position, contains_obstacle)

    def display(self, guard_position: Position | None) -> None:
        """
        Displays the map in a readable format.

        Args:
            guard_position (Position): The current position of the guard.
        """
        logging.info("Display Map:")
        if guard_position:
            for row in self._coordinate_mapping:
                logging.info(
                    "".join(
                        (
                            "G"
                            if coordinate.position == guard_position
                            else str(coordinate)
                        )
                        for coordinate in row
                    )
                )
        else:
            for row in self._coordinate_mapping:
                logging.info("".join(str(coordinate) for coordinate in row))

    def is_valid_position(self, position: Position) -> bool:
        """
        Checks if the given position is valid within the map boundaries.

        Args:
            position (Position): The position to check.

        Returns:
            bool: True if the position is valid, False otherwise.
        """
        return 0 <= position.y < len(
            self._coordinate_mapping
        ) and 0 <= position.x < len(self._coordinate_mapping[0])

    def get_coordinate(self, position: Position) -> Coordinate:
        """
        Retrieves the coordinate at the given position.

        Args:
            position (Position): The position of the coordinate.

        Returns:
            Coordinate: The coordinate at the specified position.
        """
        return self._coordinate_mapping[position.y][position.x]

    def visit_coordinate(self, position: Position, direction: str):
        """
        Marks the coordinate at the given position as visited in the specified direction.

        Args:
            position (Position): The position of the coordinate.
            direction (str): The direction to move ('^', 'v', '<', '>').
        """
        if not self.is_valid_position(position):
            raise ValueError(f"Guard position {position} is out of bounds.")
        if self.get_coordinate(position).contains_obstacle:
            raise ValueError(f"Guard position {position} cannot be on an obstacle.")
        if direction not in ["^", "v", "<", ">"]:
            logging.error(f"Invalid direction: {direction}. Must be one of '^', 'v', '<', '>'.")
            logging.error(f"Position: {position}, Coordinate: {self.get_coordinate(position)}")
            raise ValueError(
                f"Invalid direction: {direction}. Must be one of '^', 'v', '<', '>'."
            )
        self.get_coordinate(position).visit(direction)


class Guard:
    """Abstract base class for the Guard in the Guard Gallivant puzzle."""

    def __init__(self, position: Position, direction: str):
        """
        Initializes the Guard object.

        Args:
            position (Position): The starting position of the guard.
            direction (str): The initial direction of the guard ('^', 'v', '<', '>').
        """
        self.position = position
        self.previous_direction = None
        self.direction = direction
        self.direction_map = {
            "^": Position(0, -1),
            "v": Position(0, 1),
            "<": Position(-1, 0),
            ">": Position(1, 0),
        }

    def handle_obstacle_encounter(self, coordinate_mapping: CoordinateMapping) -> str:
        """
        Turns the guard 90 degrees clockwise.
        """
        directions = ["^", ">", "v", "<"]
        self.previous_direction = self.direction
        self.direction = directions[
            (directions.index(self.direction) + 1) % len(directions)
        ]

    def patrol(self, coordinate_mapping: CoordinateMapping):
        """
        Defines the patrolling behavior of the guard.

        Args:
            coordinate_mapping (CoordinateMapping): The map of coordinates.
        """
        move = self.direction_map[self.direction]
        new_position = Position(self.position.x + move.x, self.position.y + move.y)

        if not coordinate_mapping.is_valid_position(new_position):
            self.position = None
            self.direction = None
            return

        if coordinate_mapping.get_coordinate(new_position).contains_obstacle:
            # logging.info(f"Guard encountered an obstacle at {new_position}.")
            self.handle_obstacle_encounter(coordinate_mapping)
        else:
            self.position = new_position        
            
        if self.position is None or self.direction is None:
            logging.error("Guard position or direction is None, cannot visit coordinate.")
            logging.error(
                f"Current position: {self.position}, Current direction: {self.direction}, Previous direction: {self.previous_direction}"
            )
            return
        
        coordinate_mapping.visit_coordinate(self.position, self.direction)

    def has_left_the_mapped_area(self) -> bool:
        """
        Checks if the guard has left the mapped area.

        Returns:
            bool: True if the guard has left the mapped area, False otherwise.
        """
        return self.position is None

    def get_position(self):
        return self.position

    def get_direction(self):
        return self.direction

    def set_position(self, position: Position):
        """
        Sets the guard's position.

        Args:
            position (Position): The new position of the guard.
        """
        self.position = position

    def set_direction(self, direction: str):
        """
        Sets the guard's direction.

        Args:
            direction (str): The new direction of the guard ('^', 'v', '<', '>').
        """
        if direction not in self.direction_map:
            raise ValueError(f"Invalid direction: {direction}")
        self.direction = direction


class InputReader:
    """Handles reading and parsing of the puzzle input file."""

    def _read_file(self, filename: str) -> List[str]:
        """
        Reads the content of the input file.

        Args:
            filename (str): The path to the input file.

        Returns:
            List[str]: A list of strings, each representing a line in the file.

        Raises:
            FileNotFoundError: If the file does not exist.
            IOError: If there is an error reading the file.
        """
        try:
            with open(filename, "r") as file:
                return [line.strip() for line in file if line.strip()]
        except FileNotFoundError:
            raise FileNotFoundError(f"Error: The file {filename} does not exist.")
        except IOError as e:
            raise IOError(f"Error reading file {filename}: {e}")

    def get_start_values(
        self, filename: str = "day6/puzzle_input"
    ) -> Tuple[List[List[Coordinate]], Tuple[List[int], str]]:
        """
        Parses the input file and returns the map as a 2D list of Coordinate objects
        along with the guard's starting position and direction.

        Args:
            filename (str): The path to the input file.

        Returns:
            Tuple[List[List[Coordinate]], Tuple[List[int], str]]:
                - A 2D list where each inner list represents a row of Coordinate objects.
                - A tuple containing the guard's starting position (as a list of two integers) and direction (as a string).

        Raises:
            FileNotFoundError: If the file does not exist.
            IOError: If there is an error reading the file.
            ValueError: If the file content is empty or invalid.
        """
        if not filename:
            raise ValueError("Filename cannot be empty.")
        if not isinstance(filename, str):
            raise TypeError("Filename must be a string.")

        file_content = self._read_file(filename)

        if not file_content:
            raise ValueError(f"No valid content in the file {filename}")

        input_data = [list(line) for line in file_content]

        if not input_data:
            raise ValueError(
                f"The file {filename} is empty or contains no valid content."
            )

        guard_position, guard_direction = self.find_guard(input_data)
        return CoordinateMapping(input_data=input_data), Guard(
            position=guard_position, direction=guard_direction
        )

    def find_guard(self, input_data: List[List[str]]) -> Tuple[Position, str]:
        """
        Finds the guard's starting position and direction on the map.

        Args:
            input_data (List[List[str]]): The 2D list representing the map.

        Returns:
            Tuple[List[int], str]: The guard's starting position and direction.

        Raises:
            ValueError: If the guard's starting position is not found.
        """
        for i, row in enumerate(input_data):
            for j, cell in enumerate(row):
                if cell in "^<>v":
                    return Position(x=j, y=i), cell
        raise ValueError("Guard starting position not found on the map.")


class GallivantGuardEngine:
    """Engine for managing the Guard Gallivant puzzle."""

    def __init__(self, name: str, coordinate_mapping: CoordinateMapping, guard: Guard):
        """
        Initializes the GallivantGuardEngine with a name, coordinate mapping, and guard.

        Args:
            name (str): The name of the engine.
            coordinate_mapping (CoordinateMapping): The coordinate mapping for the puzzle.
            guard (Guard): The guard object representing the guard's position and direction.

        Raises:
            TypeError: If `coordinate_mapping` is not a CoordinateMapping or `guard` is not a Guard.
        """
        if not isinstance(coordinate_mapping, CoordinateMapping):
            raise TypeError(
                "coordinate_mapping must be an instance of CoordinateMapping."
            )
        if not isinstance(guard, Guard):
            raise TypeError("guard must be an instance of Guard.")

        self.name = name
        self.coordinate_mapping = coordinate_mapping
        self.guard = guard

    def __str__(self):
        return f"GallivantGuardEngine(name={self.name})"

    def __repr__(self):
        return self.__str__()

    def start(self):
        """Starts the engine."""
        # logging.info(f"{self.name} is starting...")
        if self.guard.position is not None and self.guard.direction is not None:
            self.coordinate_mapping.visit_coordinate(
                self.guard.position, self.guard.direction
            )
        while not self.guard.has_left_the_mapped_area():
            #logging.info(
            #    f"Guard {self.guard.get_position()} is patrolling in direction {self.guard.get_direction()}."
            #)
            self.guard.patrol(self.coordinate_mapping)
            if self.loop_detected():
                # logging.info("Loop detected, stopping the engine.")
                break

    def stop(self):
        """Stops the engine."""
        logging.info(f"{self.name} is stopping...")

    def loop_detected(self):
        """
        Detects loops in the specified direction.

        Args:
            direction (str): The direction to check ('^', 'v', '<', '>').
        """

        if not self.guard.get_position():
            # logging.info("Guard has left the mapped area.")
            return False

        coordinate: Coordinate = self.coordinate_mapping.get_coordinate(
            self.guard.get_position()
        )

        if coordinate.has_been_visited_in_direction_before(self.guard.direction):
            # logging.warning(
            #    f"Loop detected at {coordinate.position} in direction {self.guard.direction}."
            # )
            return True

        return False

    def get_visited_coordinates(self) -> List[Coordinate]:
        """
        Retrieves the list of visited coordinates.

        Returns:
            List[Coordinate]: A list of Coordinate objects that have been visited.
        """
        visited_coordinates = []
        for row in self.coordinate_mapping._coordinate_mapping:
            for coordinate in row:
                if coordinate.has_been_visited():
                    visited_coordinates.append(coordinate)
        return visited_coordinates

    def get_number_of_visited_coordinates(self) -> int:
        """
        Retrieves the number of visited coordinates.

        Returns:
            int: The number of Coordinate objects that have been visited.
        """
        return len(self.get_visited_coordinates())

    def display_map(self):
        logging.info(f"Guard {self.guard.get_position()}")
        self.coordinate_mapping.display(self.guard.get_position())


class LoopObstructionFinder:
    """Finder for loop obstructions in the Guard Gallivant puzzle."""

    def __init__(self, coordinate_mapping: CoordinateMapping, guard: Guard, initial_coordinate_list: List[Coordinate]):
        """
        Initializes the LoopObstructionFinder with a coordinate mapping and a guard.

        Args:
            coordinate_mapping (CoordinateMapping): The coordinate mapping for the puzzle.
        """
        self.coordinate_mapping = coordinate_mapping
        self.guard = guard
        self.initial_guard_position = guard.get_position()

        # Remove the initial guard position from the coordinate list
        if not self.initial_guard_position:
            raise ValueError("Guard position is None.")

        self.coordinate_list = [
            coord
            for coord in initial_coordinate_list
            if not coord.position == self.initial_guard_position
        ]

    def find_obstructions(self) -> List[Coordinate]:
        """
        Finds and returns a list of coordinates where obstructions can be placed to create infinite loops.

        Returns:
            List[Coordinate]: A list of Coordinate objects that are obstructions.
        """
        obstruction_coordinates = []

        def test_obstruction(coordinate: Coordinate) -> Coordinate | None:
            # logging.info(f"Testing obstruction at {coordinate.position}...")
            test_mapping = deepcopy(self.coordinate_mapping)
            test_mapping._coordinate_mapping[coordinate.position.y][
                coordinate.position.x
            ].add_obstacle()

            test_engine = GallivantGuardEngine(
                name="Loop Obstruction Finder Engine",
                coordinate_mapping=test_mapping,
                guard=deepcopy(self.guard),
            )
            test_engine.start()

            if not test_engine.guard.has_left_the_mapped_area():
                logging.info(
                    f"Loop created by inserting obstacle at {coordinate.position}."
                )
                return coordinate
            return None

        with ThreadPoolExecutor() as executor:
            results = executor.map(test_obstruction, self.coordinate_list)

        obstruction_coordinates = [coord for coord in results if coord is not None]
        return obstruction_coordinates

    def get_number_of_possible_obstructions(self) -> int:
        """
        Retrieves the number of possible obstructions for infinite loops in the coordinate mapping.

        Returns:
            int: The number of Coordinate objects that are obstructions.
        """
        return len(self.find_obstructions())


if __name__ == "__main__":
    # Example usage

    filename = "day6/small_puzzle_input"
    filename = "day6/puzzle_input"
    input_reader = InputReader()
    coordinate_mapping, guard = input_reader.get_start_values(filename=filename)
    engine = GallivantGuardEngine(
        name="Guard Gallivant Engine",
        coordinate_mapping=deepcopy(coordinate_mapping),
        guard=deepcopy(guard),
    )

    #engine.display_map()
    engine.start()
    engine.display_map()
    logging.info(engine.get_number_of_visited_coordinates())

    loop_obstruction_finder = LoopObstructionFinder(coordinate_mapping=coordinate_mapping, guard=guard, initial_coordinate_list=engine.get_visited_coordinates())
    obstructions = loop_obstruction_finder.find_obstructions()
    logging.info(obstructions)
    logging.info(f"Number of possible obstructions: {len(obstructions)}")
    logging.info(f"Number of visited coordinates in original map: {engine.get_number_of_visited_coordinates()}")
