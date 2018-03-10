# manager of tournament
import options
import children
import subprocess
from random import sample
import os
from time import sleep as wait


def list_to_str(list):
    return ",".join(str(x) for x in list)


class Director:
    def __init__(self):
        self.options = options.Options()
        self.children = children.Children(self.options)
        self.networks = [0 for x in range(self.options.network_count)]

    def store_performances(self):
        os.chdir(os.path.dirname(self.options.network_manager))
        run_str = "{} {} {}".format(self.options.network_manager, 1, self.wins)
        result = subprocess.getstatusoutput(run_str)
        print(result[1])
        print("\n result was: {}".format(result[0]))

    def evolve_networks(self):
        run_str = "{} {}".format(self.options.network_manager, 3)
        result = subprocess.getstatusoutput(run_str)
        print(result[1])
        print("\n result was: {}".format(result[0]))

    def run(self):
        for id in range(self.options.network_count):
            opponent_ids = sample(range(self.options.network_count), k=3)
            match_results = self.children.run(id, opponent_ids)

            for game in match_results:
                winner = game['winner']
                if winner is None:
                    continue

                self.networks[winner] += 1
        self.wins = list_to_str(self.networks)
        print("performance string is {}".format(self.wins))
        print(self.networks)
        self.store_performances()
        self.evolve_networks()

    def idle(self):
        while self.options.run == 1:
            print(" directing a tournament \n")
            self.run()
            print(" finished that tournament \n")
            self.options.check_run()
            print(" looping \n")


if __name__ == "__main__":
    director = Director()
    director.idle()
