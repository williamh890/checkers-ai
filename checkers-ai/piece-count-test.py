import os
import sys
from multiprocessing import Pool
import subprocess

DEPTH = 4
#NUM_GAMES is number of games played as each color
NUM_GAMES = 50


def run_games_vs_piece_count(network_id, games):
    pool = Pool(processes=8)

    out = pool.map_async(
        run_game,
        [(network_id, -1) for _ in range(games)]+[(-1, network_id) for _ in range(games)]
    ).get()

    wins = 0
    losses = 0
    for game in out: 
        winner = game['winner']

        if winner is None:
            parsedWinner = 'Draw'
        elif winner != -1:
            parsedWinner = 'Network'
        else:
            parsedWinner = 'Piece Count'

        #print("winner is {}".format(parsedWinner))
        if game['winner'] == network_id:
            wins += 1
        elif game['winner'] == -1:
            losses += 1
    print("Network: {} Piece count: {} Out of {} games".format(wins, losses, games *2))


def run_game(args):
    red_id, black_id = args
    prog_name = os.path.join(os.getcwd(), "checkers.out")

    run_str = "{} {} {} {}".format(
        prog_name, red_id, black_id, DEPTH
    )

    winner, program_output = subprocess.getstatusoutput(run_str)

    if winner == 139:
        print("Error")
        print(program_output)
    game_result = {
        1: red_id,
        0: None,
        255: black_id
    }[winner]

    return {
        "red": red_id,
        "black": black_id,
        "winner": game_result
    }


if __name__ == "__main__":
    try:
        network_id = sys.argv[1]
    except:
        network_id = 0
    print ("Running Games...")
    run_games_vs_piece_count(network_id, games=NUM_GAMES)
