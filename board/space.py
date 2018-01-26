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

    def place_checkers(self, red_player, black_player):
        for red_checker, black_checker in zip(red_player.checkers, black_player.checkers):
            self.spaces[red_checker.space].checker_to(red_checker)
            self.spaces[black_checker.space].checker_to(black_checker)

class Space():
    def __init__(self, id):
        self.id = id
        self.checker = None

    def get_checker_color(self): # returns checker color, and maybe empty string if none?
        return checker.color if checker else ""
        # return self.checker_color if checker_color else ""

    def checker_to(self, checker):
        self.checker = checker
