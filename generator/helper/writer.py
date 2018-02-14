# writer.py
# Author: Hal DiMarchi
# writer of games
import json
from . import get_random_string


def write_game(game_record):
    rand = get_random_string()
    with open("boards/game{}.json".format(rand), 'w') as f:
        json.dump(game_record, f)
