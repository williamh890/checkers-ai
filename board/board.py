# board.py
# Authors: Hal DiMarchi
# checkers board class

from move_generator import get_move_generator
import checker
import space
import player

BLACK = "black"
RED = "red"

class Board(object):
    def __init__(self):
        self.move_generator = get_move_generator('../generator/move-table.json')
        self.make_spaces()
        self.make_checkers()
        self.active_player = self.black_player

    def make_spaces(self):
        self.board_spaces = space.Spaces()

    def make_checkers(self):
        self.red_player = player.Player(RED)
        self.black_player = player.Player(BLACK)
        self.board_spaces.place_checkers(self.red_player, self.black_player)

    def turn(self):
        self.active_player.consider_moves()
        pass

    def draw(self):  # some tklinter stuff, or maybe using separate gui package
        pass

if __name__ == "__main__":
    game_board = Board()
