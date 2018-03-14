# manager of individual games using multiprocessing
from multiprocessing import Pool, Process
import os
import subprocess


def run_child(args):
    os.chdir(os.path.dirname(args[0]))
    prog_name, redId, blackId = args

    run_str = "{} {} {}".format(prog_name, redId, blackId)
    result = subprocess.getstatusoutput(run_str)
    prog_output, winner = result[:2]

    print(result[0])
    game_result = {
        1: redId,
        0: None,
        255: blackId
    }[result[0]]

    print(result[1])

    return {
        "red": redId,
        "black": blackId,
        "winner": game_result
    }


class Children:
    def __init__(self, options):
        self.options = options

    def get_children(self):
        self.children = Pool(processes=self.options.max_processes)

    def run(self, id, opponent_ids):
        match_results = self.children.map_async(
            run_child,
            [
                (self.options.checkers_game, id, opponent_id)
                for opponent_id in opponent_ids
            ]
        )

        return match_results.get()

    def destroy_children(self):
        self.children.close()
        self.children.join()
