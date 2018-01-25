# player.py
# Author: Hal DiMarchi
# player class for chess

from checker import Checker

START = {"red": [0,12], "black": [20,32]}

class Player():
    def __init__(self, color):
        self.color = color
        self.make_checkers(*START[color])

    def make_checkers(self, start_space, end_space):
        self.checkers = []
        for i in range(start_space, end_space):
            self.checkers.append(Checker(i - start_space, self.color, i))

    def consider_moves(self):
        pass
