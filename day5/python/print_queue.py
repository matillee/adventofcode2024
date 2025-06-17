"""
This module handles the processing of a print queue based on page ordering rules
and safety manual updates. It verifies the updates, applies the rules, and calculates
the sum of middle page numbers in the print queue.
"""

from typing import List, Tuple


class PuzzleInput:
    """Handles reading and parsing of the print queue input file."""

    def __init__(self, filename: str = "day5/puzzle_input"):
        """
        Initializes the PuzzleInput object and reads the file content.

        Args:
            filename (str): The path to the input file. Defaults to "day5/puzzle_input".

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

    def parse_input(self) -> Tuple[List[Tuple[int, int]], List[List[int]]]:
        """
        Parses the input file into page ordering rules and safety manual updates.

        Returns:
            Tuple[List[Tuple[int, int]], List[List[int]]]: A tuple containing:
                - A list of tuples representing page ordering rules.
                - A list of lists representing safety manual updates.

        Raises:
            ValueError: If the file content is empty or has an unexpected structure.
        """
        page_ordering_rules: List[Tuple[int, int]] = []
        safety_manual_updates: List[List[int]] = []

        if not self.file_content:
            raise ValueError(f"No valid content in the file {self.filename}")

        for line in self.file_content:
            if '|' in line:
                page_ordering_rules.append(tuple(map(int, line.split('|'))))
            elif ',' in line:
                safety_manual_updates.append(list(map(int, line.split(','))))
            else:
                raise ValueError(f"Unexpected structure of the line found in the file: {line}")

        return page_ordering_rules, safety_manual_updates


class PrintQueue:
    """Handles the print queue operations based on the parsed input."""

    def __init__(self, page_ordering_rules: List[Tuple[int, int]], safety_manual_updates: List[List[int]]):
        """
        Initializes the PrintQueue object with page ordering rules and safety manual updates.

        Args:
            page_ordering_rules (List[Tuple[int, int]]): A list of tuples representing page ordering rules.
            safety_manual_updates (List[List[int]]): A list of lists representing safety manual updates.
        """
        self.page_ordering_rules = page_ordering_rules
        self.safety_manual_updates = safety_manual_updates
        self.print_queue: List[List[int]] = []
        self.incorrectly_ordered_updates: List[List[int]] = []
        self.corrected_updates_print_queue: List[List[int]] = []

    def update_order_is_valid(self, update: List[int]) -> bool:
        """
        Checks if a safety manual update is valid based on the page ordering rules.

        Args:
            update (List[int]): A list of page numbers in the update.
            append_to_incorrectly_ordered (bool): Whether to append invalid updates to the incorrectly ordered list.

        Returns:
            bool: True if the update is valid, False otherwise.
        """
        for rule in self.page_ordering_rules:
            if rule[0] in update and rule[1] in update:
                if update.index(rule[0]) > update.index(rule[1]):
                    return False
        return True

    def calculate_middle_page_sum(self, print_queue: List[List[int]]) -> int:
        """
        Calculates the sum of the middle page numbers in the print queue.

        Args:
            print_queue (List[List[int]]): The print queue containing lists of page numbers.

        Returns:
            int: The sum of the middle page numbers.
        """
        total_sum = 0
        for update in print_queue:
            mid_index = len(update) // 2
            if len(update) % 2 == 0:
                total_sum += (update[mid_index - 1] + update[mid_index]) / 2
            else:
                total_sum += update[mid_index]
        return total_sum

    def verify_safety_manual_updates(self):
        """
        Verifies the safety manual updates against the page ordering rules
        and adds valid updates to the print queue.
        """
        for update in self.safety_manual_updates:
            if self.update_order_is_valid(update):
                self.print_queue.append(update)
            else:
                self.incorrectly_ordered_updates.append(update)

    def process_print_queue(self) -> int:
        """
        Processes the print queue by verifying updates and calculating the middle page sum.

        Returns:
            int: The sum of the middle page numbers in the print queue.
        """
        self.verify_safety_manual_updates()
        return self.calculate_middle_page_sum(self.print_queue)

    def correct_incorrectly_ordered_updates(self) -> int:
        """
        Corrects the incorrectly ordered updates and adds them to the corrected updates print queue.

        Returns:
            int: The sum of the middle page numbers in the corrected updates print queue.
        """
        for update in self.incorrectly_ordered_updates:
            self.apply_rules_to_incorrectly_ordered_updates(update)
        return self.calculate_middle_page_sum(self.corrected_updates_print_queue)

    def apply_rules_to_incorrectly_ordered_updates(self, update: List[int]):
        """
        Applies the page ordering rules to an incorrectly ordered update.

        Args:
            update (List[int]): A list of page numbers in the update.
        """
        for rule in self.page_ordering_rules:
            if rule[0] in update and rule[1] in update:
                if update.index(rule[0]) > update.index(rule[1]):
                    index_first = update.index(rule[0])
                    index_second = update.index(rule[1])
                    update[index_first], update[index_second] = update[index_second], update[index_first]
        if self.update_order_is_valid(update):
            self.corrected_updates_print_queue.append(update)
        else:
            self.apply_rules_to_incorrectly_ordered_updates(update)

if __name__ == "__main__":
    # Example usage
    puzzle_input = PuzzleInput()
    page_ordering_rules, safety_manual_updates = puzzle_input.parse_input()

    print_queue = PrintQueue(page_ordering_rules, safety_manual_updates)
    middle_page_sum = print_queue.process_print_queue()
    print(f"Sum of middle page numbers in the print queue: {middle_page_sum}")

    print(f"Processing {len(print_queue.incorrectly_ordered_updates)} number of incorrectly ordered updates...")

    corrected_updates_middle_page_sum = print_queue.correct_incorrectly_ordered_updates()
    print(f"Number of corrected updates: {len(print_queue.corrected_updates_print_queue)}")
    print(f"Sum of middle page numbers in the corrected updates print queue: {corrected_updates_middle_page_sum}")

