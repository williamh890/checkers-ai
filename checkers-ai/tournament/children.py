# manager of individual games using multiprocessing
from multiprocessing import Pool, Process
import os
import subprocess


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
    prog_name, redId, blackId = args

    run_str = "{} {} {}".format(prog_name, redId, blackId)
    #print(run_str)

    winner, program_output = subprocess.getstatusoutput(run_str)
    #print(program_output)

    if winner == 139:
        print("game segfaulted")
        print("network {} or {} may be corrupted".format(redId, blackId))
        raise Exception("Segmentation Fault in checkers.out")

    game_result = {
        1: redId,
        0: None,
        255: blackId
    }[winner]

    return {
        "red": redId,
        "black": blackId,
        "winner": game_result
    }
