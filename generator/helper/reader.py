# reader.py
# Author: Hal DiMarchi
# reader of games

import json
import os


class Reader:
    def __init__(self, board_dir="boards/"):
        self.board_dir = board_dir

    def get_file_from_command_line(self):
        self.name = input("Enter a file name")

    def get_file(self, name):
        self.name = name

    def get_game(self):
        game_path = os.path.join(self.board_dir, "{}.json".format(self.name))
        with open(game_path, 'r') as f:
            self.game = json.loads(f.read())
