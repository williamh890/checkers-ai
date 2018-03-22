import options
import children
import subprocess
from random import sample
import os
import time


def list_to_str(list):
    return ",".join(str(x) for x in list)


class Director:
    def __init__(self):
        self.options = options.Options()
        self.children = children.Children(self.options)
        self.networks = [0 for x in range(self.options.network_count)]

    def idle(self):
        self.children.get_children()

        print(" directing a tournament")
        self.run()
        print(" finished that tournament")
        print(" storing performances and evolving")
        self.store_performances()
        self.evolve_networks()
        self.options.check_run()
        print("run is {}".format(self.options.run))
        print(" looping")

    def run(self):
        ids = range(self.options.network_count)
        matchups = self.get_matchups(ids)

        results = self.children.run(matchups)

        for game in results:
            winner = game['winner']
            if winner is None:
                continue

            self.networks[winner] += 1

        self.wins = list_to_str(self.networks)
        print("performance string is {}".format(self.wins))
        self.networks = [0 for x in range(self.options.network_count)]

    def get_matchups(self, ids):
        matchups = []
        for id in ids:
            opponent_ids = sample(ids, k=3)

            network_matchups = [
                (self.options.checkers_game, id, opponent_id)
                for opponent_id in opponent_ids
            ]

            matchups += network_matchups
        print(matchups)
        return matchups

    def store_performances(self):
        os.chdir(os.path.dirname(self.options.network_manager))
        run_str = "{} {} {}".format(self.options.network_manager, 1, self.wins)
        result = subprocess.getstatusoutput(run_str)
        print(result[1])
        print("result was: {}".format(result[0]))

    def evolve_networks(self):
        run_str = "{} {}".format(self.options.network_manager, 3)
        result = subprocess.getstatusoutput(run_str)
        print(result[1])
        print("result was: {}".format(result[0]))


if __name__ == "__main__":
    director = Director()
    start = time.time()
    director.idle()
    end = time.time()
    print(f"total time was {end - start}")
