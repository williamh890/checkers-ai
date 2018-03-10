# manager of individual games using multiprocessing
from multiprocessing import Pool, Process
import os
import subprocess


def run_child(args):
    os.chdir(os.path.dirname(args[0]))
    prog_name, redId, blackId = args

    print("id is {} opponent id is {}".format(redId, blackId))
    run_str = "{} {} {}".format(prog_name, redId, blackId)
    result = subprocess.getstatusoutput(run_str)
    prog_output, winner = result[:2]

    print(prog_output)
    print("\n result was: {}".format(winner))

    return {
        "red": redId,
        "black": blackId,
        "winner": blackId if result[0] == 1 else redId
    }


class Children:
    def __init__(self, options):
        self.options = options

    def get_children(self):
        self.children = Pool(processes=self.options.max_processes,
                             maxtasksperchild=30)

    def run(self, id, opponent_ids):
        self.get_children()
        print("opponents are {}".format(opponent_ids))
        wins = self.children.map_async(
            run_child,
            [
                (self.options.checkers_game, id, opponent_id)
                for opponent_id in opponent_ids
            ]
        )

        wins = sum([game['winner'] for game in wins.get()])

        print(wins)
        return wins
