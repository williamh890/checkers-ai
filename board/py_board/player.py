# player.py
# Author: Hal DiMarchi
# player class for chess

from checker import Checker

START = {"red": [0,12], "black": [20,32]}

class Player():
    def __init__(self, color, move_generator):
        self.color = color
        self.make_checkers(*START[color])
        self.move_generator = move_generator

    def make_checkers(self, start_space, end_space):
        self.checkers = []
        for i in range(start_space, end_space):
            self.checkers.append(Checker(i - start_space, self.color, i))

    def consider_moves(self):
        moves = {}
        jumps = {}
        for checker in self.checkers:
            jumps[str(checker.id)] = self.move_generator.get_jumps_at(checker.space)
            moves[str(checker.id)] = self.move_generator.get_moves_at(checker.space)
        return {"jumps":jumps, "moves":moves}

    def make_move(self, moves):
        print(moves)

    def make_jump(self, jumps):
        print(jumps)

    def is_my_color(self, color):
        return self.color == color
