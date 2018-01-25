# board.py
# Authors: Hal DiMarchi
# checkers board class

from move_generator import get_move_generator
import checker
import space

BLACK = "black"
RED = "red"
BOARD_SIZE = 32
RED_START = [0, 12]
BLACK_START = [20, 32]

class Board(object):
    def __init__(self):
        self.move_generator = get_move_generator('../generator/move-table.json')
        self.make_spaces()
        self.make_checkers()
        self.active_player = BLACK

    def make_spaces(self):
        self.board_spaces = space.Spaces()

    def make_checkers(self):
        self.red_checkers = checker.Checkers(RED, *RED_START)
        self.black_checkers = checker.Checkers(BLACK, *BLACK_START)
        self.board_spaces.place_checkers(self.red_checkers, self.black_checkers)

    def turn(self):
        pass

if __name__ == "__main__":
    game_board = Board()
