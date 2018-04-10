import json
import sys


def parse(fname):
    with open(fname) as f:
        log = json.loads(f.read())

    total_moves, total_time = 0, 0
    total_games = 0
    max_gen = 0
    for gen in log:
        max_gen = int(gen['session gen'])

        for game in gen['results']:
            total_games += 1
            times = game['timings']
            moves = times['moves']
            total = float(times['total'])

            total_time += total

    print(total_games, max_gen * 90)
    print('avg gen time: {:.5f} sec'.format(total_time / max_gen))
    print('avg game time: {:.5f} sec'.format(total_time / total_games))


if __name__ == "__main__":
    parse(sys.argv[1])
