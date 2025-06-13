""" 
The unusual data (your puzzle input) consists of many reports, one report per line. 
Each report is a list of numbers called levels that are separated by spaces. For example:

7 6 4 2 1
1 2 7 8 9
9 7 6 2 1
1 3 2 4 5
8 6 4 4 1
1 3 6 7 9

This example data contains six reports each containing five levels.

The engineers are trying to figure out which reports are safe. 
The Red-Nosed reactor safety systems can only tolerate levels that are 
either gradually increasing or gradually decreasing. 
So, a report only counts as safe if both of the following are true:

    The levels are either all increasing or all decreasing.
    Any two adjacent levels differ by at least one and at most three.

In the example above, the reports can be found safe or unsafe by checking those rules:

    7 6 4 2 1: Safe because the levels are all decreasing by 1 or 2.
    1 2 7 8 9: Unsafe because 2 7 is an increase of 5.
    9 7 6 2 1: Unsafe because 6 2 is a decrease of 4.
    1 3 2 4 5: Unsafe because 1 3 is increasing but 3 2 is decreasing.
    8 6 4 4 1: Unsafe because 4 4 is neither an increase or a decrease.
    1 3 6 7 9: Safe because the levels are all increasing by 1, 2, or 3.

So, in this example, 2 reports are safe.

Analyze the unusual data from the engineers. How many reports are safe? 
"""

# Check if the levels are either all increasing or all decreasing
from typing import List
class RedNosedReports:
    
    def is_safe_report(self, levels: List[int]) -> bool:
        if len(levels) < 2:
            return True  # A single level is trivially safe
        
        increasing = all(levels[i] < levels[i + 1] for i in range(len(levels) - 1))
        decreasing = all(levels[i] > levels[i + 1] for i in range(len(levels) - 1))
        
        if not (increasing or decreasing):
            return False
        
        return self.check_adjecent_levels(levels)

    def check_adjecent_levels(self, levels: List[int]) -> bool:
        """
        Check if all adjacent levels differ by at least one and at most three.
        """
        for i in range(len(levels) - 1):
            if not (1 <= abs(levels[i] - levels[i + 1]) <= 3):
                return False
        return True
    
    def is_safe_report_problem_dampener(self, levels: List[int]) -> bool:
        """
        The Problem Dampener is a reactor-mounted module that lets the reactor 
        safety systems tolerate a single bad level in what would otherwise be a safe report. 
        It's like the bad level never happened!

        Now, the same rules apply as before, 
        except if removing a single level from an unsafe report would make it safe, 
        the report instead counts as safe.
        """

        # Check if the report is already safe
        if self.is_safe_report(levels):
            return True
        # Try removing each level one by one and check if the report becomes safe
        for i in range(len(levels)):
            modified_levels = levels[:i] + levels[i + 1:]
            if self.is_safe_report(modified_levels):
                return True
        return False
    
    def count_safe_reports(self, reports: List[List[int]]) -> int:
        return sum(1 for report in reports if self.is_safe_report(report))

    def count_safe_reports_problem_dampener(self, reports: List[List[int]]) -> int:
        return sum(1 for report in reports if self.is_safe_report_problem_dampener(report))
    
    def parse_reports(self, report_lines: List[str]) -> List[List[int]]:
        reports = []
        for line in report_lines:
            try:
                levels = list(map(int, line.split()))
                reports.append(levels)
            except ValueError as e:
                print(f"Error parsing line '{line}': {e}")
        return reports
    
class PuzzleInput:
    def __init__(self, filename: str = "day2/puzzle_input"):
        self.filename = filename
        self.report_lines = self.__read_file()
        if not self.report_lines:
            raise ValueError(f"No content in the file {self.filename}")

    def __read_file(self) -> List[str]:
        try:
            with open(self.filename, "r") as file:
                return file.readlines()
        except FileNotFoundError:
            print(f"Error: The file {self.filename} does not exist.")
            return []
        except IOError as e:
            print(f"Error reading file {self.filename}: {e}")
            return []
        
    def get_report_lines(self) -> List[str]:
        return [line.strip() for line in self.report_lines if line.strip()]

if __name__ == "__main__":
    # Example usage
    report_lines = [
        "7 6 4 2 1",
        "1 2 7 8 9",
        "9 7 6 2 1",
        "1 3 2 4 5",
        "8 6 4 4 1",
        "1 3 6 7 9"
    ]
    
    red_nosed_reports = RedNosedReports()
    reports = red_nosed_reports.parse_reports(report_lines)
    safe_count = red_nosed_reports.count_safe_reports(reports)
    
    print(f"Number of safe reports: {safe_count}")

    # Using PuzzleInput to read from a file
    puzzle_input = PuzzleInput()
    report_lines_from_file = puzzle_input.get_report_lines()
    reports_from_file = red_nosed_reports.parse_reports(report_lines_from_file)
    safe_count_from_file = red_nosed_reports.count_safe_reports(reports_from_file)
    print(f"Number of safe reports from file: {safe_count_from_file}")

    problem_dampener_safe_count_from_file = red_nosed_reports.count_safe_reports_problem_dampener(reports_from_file)
    print(f"Number of safe reports from file (UPDATED): {problem_dampener_safe_count_from_file}")
    