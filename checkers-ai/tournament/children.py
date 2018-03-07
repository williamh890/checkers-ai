# manager of individual games using multiprocessing
from multiprocessing import Pool, Process
import os
import subprocess


def run_child(args):
    os.chdir(os.path.dirname(args[0]))
    run_str = "{} {} {}".format(args[0], args[1], args[2])
    result = subprocess.run(run_str,
                            shell=True,
                            stdout=subprocess.PIPE)
    print(result.returncode)
    return result


class Children:
    def __init__(self, options):
        self.options = options

    def get_children(self):
        self.children = Pool(processes=self.options.max_processes,
                             maxtasksperchild=1)

    def run(self, id, opponent_ids):
        self.get_children()
        self.children.map_async(
            run_child,
            [(self.options.checkers_game,
              id, opponent_id) for opponent_id in opponent_ids]
        )
        self.children.close()
        self.children.join()
