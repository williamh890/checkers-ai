# writer.py
# Author: Hal DiMarchi
# writer of games
import json
import os
from . import get_random_string, ensure_dir


def write_game(game_record):
    directory = "boards/"
    ensure_dir(directory)
    with open(os.path.join(directory,
                           "game{}.json".format(get_random_string())),
              'w') as f:
        json.dump(game_record, f)
