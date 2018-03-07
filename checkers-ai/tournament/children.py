# manager of individual games using multiprocessing
import multiprocessing
import os
import subprocess


class Children:
    def __init__(self, options):
        self.options = options

    def run(self):
        os.chdir(os.path.dirname(self.options.checkers_game))
        run_str = "{} {} {}".format(self.options.checkers_game, 0, 1)
        result = subprocess.run(run_str,
                                shell=True,
                                stdout=subprocess.PIPE)
        print(result.returncode)
