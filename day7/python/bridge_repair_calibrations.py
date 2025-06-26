import logging
import pprint
from typing import List

# Configure logging
logging.basicConfig(
    level=logging.INFO, format="%(asctime)s - %(levelname)s - %(message)s"
)


class CalibrationEquation:

    def __init__(self, equation: str):
        self.equation = equation.strip()
        self.test_value, self.numbers_in_equation = self.parse_equation()

    def parse_equation(self):
        """
        Parses the calibration equation into a test value and a list of numbers.

        Returns:
            Tuple[int, List[int]]: A tuple containing the test value and a list of numbers in the equation.

        Raises:
            ValueError: If the equation format is invalid or contains fewer than two numbers.
        """
        test_value, right_hand_side = self.equation.split(":")
        numbers_in_equation = [
            int(num) for num in right_hand_side.strip().split() if num.isdigit()
        ]
        if len(numbers_in_equation) < 2:
            raise ValueError(
                f"Invalid equation format: {self.equation}. Expected at least two numbers in the equation."
            )
        return int(test_value.strip()), numbers_in_equation

    def __str__(self):
        return f"{self.test_value} : {self.numbers_in_equation}"

    def __repr__(self):
        return f"CalibrationEquation(test_value={self.test_value}, numbers_in_equation={self.numbers_in_equation})"


class PuzzleInput:

    def get_calibration_equations(
        self, filename: str = "day7/puzzle_input"
    ) -> List[CalibrationEquation]:
        """
        Reads the calibration equations from a file and returns a list of CalibrationEquation objects.

        Args:
            filename (str): The path to the input file. Defaults to "day7/puzzle_input".

        Returns:
            List[CalibrationEquation]: A list of CalibrationEquation objects parsed from the file.

        Raises:
            FileNotFoundError: If the file does not exist.
            IOError: If there is an error reading the file.

        Note:
            The input file should contain one equation per line in the format "test_value : number1 number2 ...".
        """
        input_data = self.read_file(filename)
        return [CalibrationEquation(line) for line in self.parse(input_data)]

    def parse(self, input_data: List[str]) -> List[str]:
        return [line.strip() for line in input_data.splitlines() if line.strip()]

    def read_file(self, filename):
        try:
            with open(filename, "r") as file:
                return file.read()
        except FileNotFoundError:
            raise FileNotFoundError(f"File {filename} not found.")
        except IOError as e:
            raise IOError(f"Error reading file {filename}: {e}")


class Operator:
    def __init__(self, operator):
        self.operator = operator

    def apply(self, a: int, b: int) -> int:
        """
        Applies the operator to two numbers.

        Args:
            a (int): The first number.
            b (int): The second number.

        Returns:
            int: The result of applying the operator to the two numbers.

        Raises:
            ValueError: If the operator is unknown.
        """
        if self.operator == "||":
            return int(str(a) + str(b))
        elif self.operator == "+":
            return a + b
        elif self.operator == "-":
            return a - b
        elif self.operator == "*":
            return a * b
        elif self.operator == "/":
            return a / b
        else:
            raise ValueError(f"Unknown operator: {self.operator}")

    def __str__(self):
        return f"{self.operator}"

    def __repr__(self):
        return f"Operator(operator={self.operator})"


class TreeNode:

    def __init__(
        self,
        equation_number: int,
        value: int,
        operator: str | None,
        tree_level: int = 0,
    ):
        self.equation_number = (
            equation_number  # Store the original number from the equation
        )
        self.value = value
        self.operator = operator  # Store the operator used to create this node
        self.tree_level = tree_level
        self.children = []

    def add_child(self, child_node: "TreeNode"):
        """
        Adds a child node to the current node.

        Args:
            child_node (TreeNode): The child node to add.
        """
        self.children.append(child_node)

    def __str__(self):
        return f"Node(value={self.value}, operator={self.operator}, level={self.tree_level})"
        return f"TreeNode(value={self.value}, operator={self.operator}, level={self.tree_level}, children={len(self.children)})"
        self.children_repr = ", ".join([str(child) for child in self.children])
        return f"TreeNode(orig={self.equation_number}, value={self.value}, operator={self.operator}, level={self.tree_level}, children=[{self.children_repr}])"

    def __repr__(self):
        self.children_repr = ", ".join([str(child) for child in self.children])
        return f"TreeNode(value={self.value}, operator={self.operator}, level={self.tree_level}, children=[{self.children_repr}])"

    def __eq__(self, other):
        if not isinstance(other, TreeNode):
            return False
        return (
            self.value == other.value
            and self.equation_number == other.equation_number
            and self.tree_level == other.tree_level
            and self.operator == other.operator
            # and self.children == other.children
        )


class EquationTree:
    def __init__(
        self,
        equation: CalibrationEquation,
        operator_list: List[Operator] | None = [Operator("+"), Operator("*")],
    ):
        """
        Initializes the EquationTree with a CalibrationEquation and an optional list of operators.
        Args:
            equation (CalibrationEquation): The calibration equation to build the tree from.
            operator_list (List[Operator], optional): A list of operators to use in the tree. Defaults to [Operator('+'), Operator('*')].
        """
        self.equation = equation
        self.operator_list = operator_list
        self.root = TreeNode(
            equation.numbers_in_equation[0],
            equation.numbers_in_equation[0],
            None,
            tree_level=0,
        )
        self.build_tree()

    def build_tree(self) -> None:
        """
        Builds a tree from the calibration equation using the specified operators.

        The tree is constructed level by level, with each node representing a value
        obtained by applying an operator to the parent node's value and a number from the equation.

        It will not create nodes that exceed the test value defined in the equation.

        Returns:
            None
        """
        if not self.root:
            logging.info("The tree is empty.")
            return

        logging.debug(f"Root node value: {self.root.value}")
        queue = [self.root]  # Use a queue for BFS

        for i in range(1, len(self.equation.numbers_in_equation)):
            next_queue = []
            for node in queue:
                for operator in self.operator_list:
                    new_value = operator.apply(
                        node.value, self.equation.numbers_in_equation[i]
                    )
                    if new_value > self.equation.test_value:
                        logging.debug(
                            f"Operator {operator} applied to node value {node.value} resulted in {new_value}, which exceeds the test value {self.equation.test_value}. Skipping."
                        )
                        continue
                    child_node = TreeNode(
                        self.equation.numbers_in_equation[i],
                        new_value,
                        str(operator),
                        tree_level=i,
                    )
                    node.add_child(child_node)
                    next_queue.append(child_node)
            queue = next_queue

    def __str__(self):
        return f"EquationTree(root={self.root})"

    def __repr__(self):
        return f"EquationTree(root={self.root})"

    def get_nodes_with_value(self, value: int) -> List[TreeNode]:
        """
        Returns all nodes with a specific value.
        Args:
            value (int): The value to search for in the tree nodes.
        Returns:
            List[TreeNode]: A list of nodes that match the specified value.
        """
        if not self.root:
            return []

        result = []

        def dfs(node):
            if node.value == value:
                result.append(node)
            for child in node.children:
                dfs(child)

        dfs(self.root)
        return result


class BridgeRepairCalibrations:

    def __init__(self, equations: List[CalibrationEquation]):
        self.equations = equations

    def get_part_one_solution(self) -> int:
        """
        Calculates the solution for part one of the puzzle.
        Returns:
            int: The total sum of valid nodes across all equations.
        """
        return self.get_solution(operator_list=[Operator("+"), Operator("*")])

    def get_part_two_solution(self) -> int:
        """
        Calculates the solution for part two of the puzzle.
        Returns:
            int: The total sum of valid nodes across all equations.
        """
        return self.get_solution(
            operator_list=[Operator("+"), Operator("*"), Operator("||")]
        )

    def get_solution(self, operator_list: List[Operator] | None = None) -> int:
        """
        Calculates the solution for the puzzle using the specified operator list.
        Args:
            operator_list (List[Operator], optional): A list of operators to use in the calculations. Defaults to None.
        Returns:
            int: The total sum of valid nodes across all equations.
        """
        total_sum = 0
        for equation in self.equations:
            logging.debug(f"Processing equation: {equation}")
            equation_tree = EquationTree(equation, operator_list)
            logging.debug(f"Equation tree built: {equation_tree}")
            possible_nodes = equation_tree.get_nodes_with_value(equation.test_value)
            if not possible_nodes:
                continue

            leaf_nodes = [
                node
                for node in possible_nodes
                if node.tree_level == len(equation.numbers_in_equation) - 1
            ]
            if len(possible_nodes) > len(leaf_nodes):
                logging.debug(
                    f"Found {len(possible_nodes)} nodes with value {equation.test_value}, but only {len(leaf_nodes)} are leaf nodes."
                )

            if len(leaf_nodes) >= 1:
                logging.debug(
                    f"Found {len(leaf_nodes)} leaf nodes with value {equation.test_value}."
                )
                total_sum += leaf_nodes[0].value

        return total_sum


if __name__ == "__main__":
    filename = "day7/puzzle_input"
    # filename = "day7/small_puzzle_input"
    puzzle_input = PuzzleInput()
    equations = puzzle_input.get_calibration_equations(filename)

    if not equations:
        logging.error("No calibration equations found.")
        exit(1)

    bridge_repair_calibrations = BridgeRepairCalibrations(equations)
    part_one_solution = bridge_repair_calibrations.get_part_one_solution()
    logging.info(f"Part One Solution: {part_one_solution}")

    part_two_solution = bridge_repair_calibrations.get_part_two_solution()
    logging.info(f"Part Two Solution: {part_two_solution}")
