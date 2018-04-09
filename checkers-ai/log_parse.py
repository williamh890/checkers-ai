import json

LOG_FILE = 'tournament.json'


def parse():
    with open(LOG_FILE) as f:
        log = json.loads(f.read())

    total_moves, total_time = 0, 0
    for gen in log:
        for game in gen['results']:
            times = game['timings']
            moves = times['moves']
            total = float(times['total'])

            total_moves += moves
            total_time += total

    print('avg move time: {:.5f} sec'.format(total_time / total_moves))


if __name__ == "__main__":
    parse()
