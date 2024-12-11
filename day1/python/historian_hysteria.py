from typing import List


class NumberDistance:

    def get_number_distance(self, first_number: int, second_number: int):
        return abs(first_number - second_number)

    def get_total_distance(self, first_list: List[int], second_list: List[int]):
        first_list.sort()
        second_list.sort()

        iteration_length = 0
        if len(first_list) < len(second_list):
            iteration_length = len(first_list)
        else:
            iteration_length = len(second_list)

        total_distance = 0
        for i in range(0, iteration_length):
            total_distance += self.get_number_distance(first_list[i], second_list[i])

        return total_distance


class PuzzleInput:

    def __init__(self, filename: str = "day1/puzzle_input"):
        self.filename = filename
        self.first_list = []
        self.second_list = []
        file_content = self.__read_file()
        if not file_content:
            raise ValueError("No content in the file {self.filename}")
        self.__parse_input(file_content)

    def __read_file(self):
        try:
            with open(self.filename, "r") as file:
                content = file.readlines()
            return content
        except FileNotFoundError:
            print(f"Error: The file {self.filename} does not exist.")
        except IOError as e:
            print(f"Error reading file {self.filename}: {e}")

    def __parse_input(self, lines: List[str]):

        for line in lines:
            numbers = line.split()
            if len(numbers) != 2:
                raise ValueError(f"Unexpected values: {numbers}")
            try:
                self.first_list.append(int(numbers[0]))
                self.second_list.append(int(numbers[1]))
            except ValueError as e:
                print(f"Error converting {numbers} to int: {e}")

        # print(self.first_list)
        # print(self.second_list)


if __name__ == "__main__":
    distance = NumberDistance().get_total_distance(
        [3, 4, 2, 1, 3, 3], [4, 3, 5, 3, 9, 3]
    )
    print(f"Total distance was {distance}")
    input = PuzzleInput()

    distance = NumberDistance().get_total_distance(input.first_list, input.second_list)
    print(f"Total distance was {distance}")
