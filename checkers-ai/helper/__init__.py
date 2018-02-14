# helper module for PyCheckersGame in checkers_game.pyx
# Author: Hal DiMarchi

import random
import string

BOARD_SIZE = 8


def get_random_string():
    random_string = ''.join([random.choice
                            (string.ascii_letters + string.digits)
                            for n in range(8)])
    return random_string


def example_board():
    board = [[0 for x in range(BOARD_SIZE)]
             for y in range(BOARD_SIZE)]

    for row in range(BOARD_SIZE):
        for column in range(BOARD_SIZE):
            if row > 2 and row < 5:
                board[row][column] = " "
            else:
                if row % 2 == 0 and column % 2 != 0:
                    if row <= 2:
                        board[row][column] = "b"
                    else:
                        board[row][column] = "r"
                elif row % 2 != 0 and column % 2 == 0:
                    if row <= 2:
                        board[row][column] = "b"
                    else:
                        board[row][column] = "r"
                else:
                    board[row][column] = " "
    return board
