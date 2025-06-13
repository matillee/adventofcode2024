import pytest

from day1.python.historian_hysteria import (
    NumberDistance,
    PuzzleInput,
)


@pytest.fixture
def number_distance():
    return NumberDistance()


@pytest.fixture
def mock_puzzle_input(tmp_path):
    # Create a temporary file for testing
    file_content = """7 2
3 8
5 6
"""
    file_path = tmp_path / "puzzle_input.txt"
    file_path.write_text(file_content)
    return PuzzleInput(filename=str(file_path))


def test_get_number_distance(number_distance):
    assert number_distance.get_number_distance(5, 3) == 2
    assert number_distance.get_number_distance(-1, 4) == 5
    assert number_distance.get_number_distance(0, 0) == 0


def test_get_total_distance(number_distance):
    list1 = [1, 2, 3]
    list2 = [4, 5, 6]
    assert number_distance.get_total_distance(list1, list2) == 9

    list1 = [1, 2]
    list2 = [4, 5, 6]
    assert number_distance.get_total_distance(list1, list2) == 6

    list1 = [1, 2, 3]
    list2 = [4]
    assert number_distance.get_total_distance(list1, list2) == 3


def test_puzzle_input(mock_puzzle_input):
    assert mock_puzzle_input.first_list == [7, 3, 5]
    assert mock_puzzle_input.second_list == [2, 8, 6]


def test_total_distance_with_puzzle_input(number_distance, mock_puzzle_input):
    total_distance = number_distance.get_total_distance(
        mock_puzzle_input.first_list, mock_puzzle_input.second_list
    )
    assert total_distance == 3
