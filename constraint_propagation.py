# Original code taken from https://github.com/aurbano/sudoku_py
# Code modified by Jonathan Helms and Harrison Allen to fit CSC 2400 project requrirements

import numpy as np
from math import sqrt
import time


class SudokuSolver:
    def __init__(self, sudoku, size):
        # Initialise the related cells cache
        self.related_cells = dict()
        self.size = size
        self.sqrt_size = int(sqrt(size))
        self.calculate_relations()

        self.analyser = SudokuAnalyser(sudoku, self.related_cells, size)

    def solve(self):
        if not self.analyser.sudoku.is_valid():
            return False

        return self.analyser.solve()

    def get_solution(self):
        return self.analyser.get_solution()

    def calculate_relations(self):
        for r in range(0, self.size):
            for c in range(0, self.size):
                coords = (c, r)
                self.related_cells[coords] = self.get_related_cells(coords)

    def get_related_cells(self, coords):
        # return a list of all cells that are constrained by this one
        # same row, col and square

        related = list()

        for i in range(0, self.size):
            related.append((i, coords[1]))
            related.append((coords[0], i))

        square_x = int((coords[0]) / self.sqrt_size) * self.sqrt_size
        square_y = int((coords[1]) / self.sqrt_size) * self.sqrt_size

        for x in range(0, self.sqrt_size):
            for y in range(0, self.sqrt_size):
                related.append((square_x + x, square_y + y))

        related_set = set(related)
        related_set.remove(coords)

        return related_set


class SudokuAnalyser:
    def __init__(self, sudoku, related_cells, size, possible_values=None):
        self.related_cells = related_cells
        self.size = size
        self.sqrt_size = int(sqrt(size))

        self.sudoku = Sudoku(sudoku, related_cells, size, possible_values)
        self.is_valid = True

        if possible_values == None:
            self.ac3_all()
            self.is_valid = self.sudoku.is_valid()

    def ac3_all(self):
        sudoku = self.sudoku
        queue = self.sudoku.get_unfinished_possible_values()

        while len(queue) > 0:
            element = queue.pop()
            element_values = self.sudoku.possible_values[element]

            # get all related cells that have definitive values set
            r_all = sudoku.related_cells[element]

            changed = False

            for related in r_all:
                if element not in sudoku.possible_values:
                    continue

                value = sudoku.grid[related[1]][related[0]]

                if value in element_values:
                    sudoku.possible_values[element].remove(value)

                    if len(sudoku.possible_values[element]) == 1:
                        (last_val,) = sudoku.possible_values[element]
                        # last element
                        self.sudoku.grid[element[1]][element[0]] = last_val
                        del self.sudoku.possible_values[element]
                        changed = True

            if changed:
                # The element changed
                related_unfinished = sudoku.get_unfinished_cells(
                    r_all
                )
                for item in related_unfinished:
                    queue.add(item)

    def apply_value(self, coord, value):
        new_possible_values = dict()
        for k, v in self.sudoku.possible_values.items():
            new_possible_values[k] = v.copy()

        new_solver = SudokuAnalyser(
            self.sudoku.grid,
            self.related_cells,
            self.size,
            new_possible_values
        )

        new_solver.is_valid = new_solver.sudoku.set_value(coord, value)

        return new_solver

    def search(self):
        queue = [(k, v) for k, v in self.sudoku.possible_values.items()]
        queue.sort(key=lambda item: len(item[1]))

        while len(queue) > 0:
            coords, poss = queue.pop(0)

            for value in poss:
                # Apply this value and analyse down this branch
                new_solver = self.apply_value(coords, value)

                if new_solver.solve():
                    self.sudoku = new_solver.sudoku
                    return True

            # Completed this branch - no valid values found
            break

        # No valid values at this depth, backtrack out
        return False

    def solve(self):
        if not self.is_valid:
            return False

        if self.sudoku.is_finished() and self.is_valid:
            # Solved!
            return True

        if not self.is_valid:
            return False

        solved = self.search()

        return solved and self.sudoku.is_finished() and self.is_valid

    def get_solution(self):
        if not self.sudoku.is_finished() or not self.is_valid:
            return np.full((self.size, self.size), -1)

        return self.sudoku.grid


class Sudoku:
    def __init__(self, grid, related_cells, size, possible_values=None):
        self.grid = np.copy(grid)
        self.related_cells = related_cells
        self.size = size
        self.sqrt_size = int(sqrt(size))

        if possible_values is None:
            self.possible_values = self.initial_possible_values()
        else:
            self.possible_values = possible_values

    # initial possible values without any pruning
    def initial_possible_values(self):
        possibilities = dict()

        for r in range(0, self.size):
            for c in range(0, self.size):
                coords = (c, r)
                cell = self.grid[coords[1]][coords[0]]

                if cell == 0:
                    possibilities[coords] = set(range(1, (self.size + 1)))

        return possibilities

    def is_finished(self):
        return len(self.possible_values) < 1

    def is_valid(self):
        # possible values must be at least one
        for _, values in self.possible_values.items():
            if len(values) == 0:
                return False

        # validate rows/cols
        for i in range(0, self.size):
            row = self.grid[i, :]
            column = self.grid[:, i]

            if (not self.valid_line(row) or not self.valid_line(column)):
                return False

        # validate squares
        for r in range(0, self.size, self.sqrt_size):
            for c in range(0, self.size, self.sqrt_size):
                square = self.grid[0+r:self.sqrt_size+r, 0+c:self.sqrt_size+c]
                if not self.valid_line(square.flatten()):
                    return False

        return True

    # set a value in a coordinate, cascading all updates for forward checking
    def set_value(self, coords, value):
        if coords not in self.possible_values or value not in self.possible_values[coords]:
            return False

        self.grid[coords[1]][coords[0]] = value
        del self.possible_values[coords]

        # update all related cells
        for related in self.related_cells[coords]:
            if related not in self.possible_values:
                if self.grid[related[1]][related[0]] == value:
                    return False

                continue

            related_values = self.possible_values[related]

            related_values.discard(value)

            if len(related_values) == 0:
                return False

            if len(related_values) == 1:
                (last_value,) = related_values
                if not self.set_value(related, last_value):
                    return False

        return True

    def get_unfinished_possible_values(self):
        return set(k for k, v in self.possible_values.items())

    def get_unfinished_cells(self, coord_set):
        return [coord for coord in coord_set if coord in self.possible_values]

    def valid_line(self, line):
        return len(line) == self.size and sum(line) == sum(set(line))


def sudoku_solver():
    """
    Solves a Sudoku puzzle and returns its unique solution.

    Input
        sudoku : 9x9 numpy array
            Empty cells are designated by 0.

    Output
        9x9 numpy array of integers
            It contains the solution, if there is one. If there is no solution, all array entries should be -1.
    """

    sudoku = [
        [13, 0, 8, 5, 0, 3, 12, 16, 9, 1, 0, 0, 4, 6, 0, 2],
	[10, 0, 0, 6, 15, 0, 2, 0, 5, 13, 11, 0, 1, 3, 0, 9],
	[2, 0, 0, 0, 0, 1, 0, 0, 3, 0, 0, 0, 0, 0, 13, 0],
	[15, 0, 11, 0, 13, 0, 0, 9, 0, 0, 0, 6, 16, 10, 7, 0],
	[0, 0, 2, 11, 8, 0, 10, 0, 0, 12, 0, 0, 13, 16, 14, 0],
	[7, 6, 3, 0, 12, 2, 1, 0, 0, 0, 4, 14, 0, 15, 0, 0],
	[0, 10, 0, 14, 0, 0, 0, 0, 8, 0, 0, 0, 0, 12, 0, 0],
	[0, 13, 15, 0, 14, 0, 0, 0, 0, 0, 0, 11, 0, 0, 0, 0],
	[0, 12, 0, 15, 0, 6, 0, 0, 0, 4, 3, 9, 0, 11, 0, 0],
	[0, 3, 0, 0, 0, 0, 5, 0, 0, 0, 6, 0, 0, 0, 15, 0],
	[11, 2, 0, 4, 0, 0, 0, 10, 0, 7, 5, 12, 9, 0, 0, 16],
	[0, 0, 0, 0, 2, 0, 3, 15, 0, 8, 10, 13, 12, 0, 4, 0],
	[6, 5, 4, 0, 0, 10, 0, 1, 0, 0, 14, 0, 0, 0, 8, 0],
	[9, 0, 16, 0, 5, 0, 14, 0, 7, 0, 0, 10, 0, 13, 6, 0],
	[14, 0, 0, 10, 3, 0, 8, 4, 0, 6, 0, 0, 7, 0, 0, 0],
	[3, 0, 0, 0, 0, 16, 0, 11, 4, 2, 12, 0, 0, 1, 0, 0]
        ];
    size = len(sudoku)

    start = time.time()
    sudoku_solver = SudokuSolver(sudoku, size)
    sudoku_solver.solve()
    print(sudoku_solver.get_solution())
    end = time.time()

    print(f"\nSolve Time: {end - start} seconds.")

if __name__ == "__main__":
    sudoku_solver()