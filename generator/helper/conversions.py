# conversion.py
# Author: Hal DiMarchi
# Conversions for PyCheckersGame
from . import BOARD_SIZE
from .checking import check_space


def one_to_two_dimensions(one_board, two_board, dimension):
    for row in range(dimension):
        for col in range(dimension):
            if check_space(row, col):
                two_board[row][col] = one_board[
                                                convert_row_col_to_index(
                                                 (row, col)
                                                                        )
                                                ]
            else:
                two_board[row][col] = " "


def convert_spaces_to_indices(spaces):
    space_nums = []
    for space in spaces:
        space_nums.append(convert_row_col_to_index(space))
    return space_nums


def convert_row_col_to_index(row_col):
    index = ((row_col[0]) * BOARD_SIZE + row_col[1])//2
    return index
