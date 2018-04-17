import sys
import os
from multiprocessing import Pool


def main(game_name):
    pool = Pool(processes=2)

    piece_count = "./skynet-game.out {} r -1 2".format(game_name)
    network = "./skynet-game.out {} b -1 9".format(game_name)
    result = pool.map_async(
        run,
        [piece_count, network]
    ).get()
    print(result)


def run(cmd):
    os.system(cmd)


if __name__ == "__main__":
    name = sys.argv[1]
    main(name)
