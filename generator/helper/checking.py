# checking.py
# Author: Hal Dimarchi
# Checking of moves and board states for PyCheckersGame


def check_space(row, column):
    if row % 2 == 0 and column % 2 != 0:
        return True
    elif row % 2 != 0 and column % 2 == 0:
        return True
        return False


def is_diagonal(mb_info):
    return not (
                mb_info[0][0] == mb_info[1][0]
                or
                mb_info[0][1] == mb_info[1][1]
                )
