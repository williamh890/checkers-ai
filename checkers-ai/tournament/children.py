from multiprocessing import Pool
import os
import subprocess
from logger import parse_game


class Children:
    def __init__(self, options):
        self.options = options

    def get_children(self):
        self.children = Pool(processes=self.options.max_processes)

    def run(self, matchups):
        match_results = self.children.map_async(
            run_child,
            matchups
        )

        return match_results.get()


def run_child(args):
    os.chdir(os.path.dirname(args[0]))
    prog_name, red_id, black_id, search_depth = args

    run_str = "{} {} {} {}".format(
        prog_name, red_id, black_id, search_depth
    )

    print(run_str)

    winner, program_output = subprocess.getstatusoutput(run_str)

    game_stats = {}

    game_result = {
        1: red_id,
        0: None,
        255: black_id
    }[winner]

    return {
        "red": red_id,
        "black": black_id,
        "winner": game_result,
        "timings": game_stats
    }
