import os
import sys
from multiprocessing import Pool
import subprocess

DEPTH = 4
NUM_GAMES = 100


def run_games_vs_piece_count(network_id, games):
    pool = Pool(processes=8)

    out = pool.map_async(
        run_game,
        [network_id for _ in range(games)]
    ).get()

    wins = 0
    for game in out:
        print("winner {}".format(game))
        if game['winner'] == 0:
            print(game['winner'])
            wins += 1

    print("{} wins vs piece count out of {} games".format(wins, games))


def run_game(red_id):
    prog_name = os.path.join(os.getcwd(), "checkers.out")

    piece_count = -1
    run_str = "{} {} {} {}".format(
        prog_name, red_id, piece_count, DEPTH
    )

    winner, program_output = subprocess.getstatusoutput(run_str)

    if winner == 139:
        print(program_output)
    game_result = {
        1: red_id,
        0: None,
        255: piece_count
    }[winner]

    return {
        "red": red_id,
        "black": piece_count,
        "winner": game_result
    }


if __name__ == "__main__":
    try:
        network_id = sys.argv[1]
    except:
        network_id = 0

    run_games_vs_piece_count(network_id, games=NUM_GAMES)
