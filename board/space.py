# spaces.py
# Author: Hal DiMarchi
# space class for checkers Board

BLACK = "black"
RED = "red"
BOARD_SIZE = 32

class Spaces():
    def __init__(self):
        self.spaces = []
        for i in range(0, BOARD_SIZE):
            self.spaces.append(Space(i))

    def place_checkers(self, red_checkers, black_checkers):
        for red_checker, black_checker in zip(red_checkers.checkers, black_checkers.checkers):
            self.spaces[red_checker.space].checker_to(red_checker.id, RED)
            self.spaces[black_checker.space].checker_to(black_checker.id, BLACK)

class Space():
    def __init__(self, id):
        self.id = id

    def precheck_for_move(self): # returns checker color, and maybe empty string if none?
        pass
        # return self.checker_color if checker_color else ""

    def checker_to(self, checker_id, checker_color):
        self.checker_id = checker_id
        self.checker_color = checker_color
