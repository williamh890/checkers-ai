# game_parser.py
# Author: Hal DiMarchi
# Class for parsing and running saved games
from time import sleep


class GameParser:
    def __init__(self, game):
        self.game = game
        print(self.game)
        self.turn = 0
        self.end_game = False

    def advance(self):
        sleep(1.00)
        try:
            self.move = self.game[self.turn]
        except Exception:
            self.end_game = True
            return False

        self.turn += 1
        if len(self.move) == 3:
            return True
        else:
            return False
