# manager of tournament
import options
import children
import subprocess
from random import sample
import os
NETWORK_COUNT = 30


def list_to_str(list):
    return ",".join(str(x) for x in list)


class Director:
    def __init__(self):
        self.options = options.Options()
        self.children = children.Children(self.options)
        self.networks = [0 for x in range(NETWORK_COUNT)]

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
        for id in range(30):
            opponent_ids = sample(range(NETWORK_COUNT), k=3)
            wins = self.children.run(id, opponent_ids)
            self.networks[id] += wins
        self.wins = list_to_str(self.networks)
        print("performance string is {}".format(self.wins))
        print(self.networks)
        self.store_performances()
        self.evolve_networks()


if __name__ == "__main__":
    director = Director()
    director.run()
    print(director.options.checkers_game)
