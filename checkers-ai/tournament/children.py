# manager of individual games using multiprocessing
from multiprocessing import Pool, Process
import os
import subprocess


def run_child(args):
    os.chdir(os.path.dirname(args[0]))
    print("id is {} opponent id is {}".format(args[1], args[2]))
    run_str = "{} {} {}".format(args[0], args[1], args[2])
    result = subprocess.getstatusoutput(run_str)
    print(result[1])
    print("\n result was: {}".format(result[0]))
    return result[0]


class Children:
    def __init__(self, options):
        self.options = options

    def get_children(self):
        self.children = Pool(processes=self.options.max_processes,
                             maxtasksperchild=1)

    def run(self, id, opponent_ids):
        self.get_children()
        print("opponents are {}".format(opponent_ids))
        wins = self.children.map_async(
            run_child,
            [(self.options.checkers_game,
              id, opponent_id) for opponent_id in opponent_ids]
        )
        self.children.close()
        self.children.join()
        wins = sum(wins.get())
        print(wins)
        return wins
