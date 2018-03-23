import options
import children
import network_git
from utils import ensure_generation_cfg
import subprocess
from random import sample
import os
import time
import json


def list_to_str(list):
    return ",".join(str(x) for x in list)


class Director:
    def __init__(self):
        self.options = options.Options()
        self.network_git = network_git.NetworkGit(self.options.networks_config,
                                                  self.options.user,
                                                  self.options.git_user,
                                                  self.options.git_password,
                                                  self.options.repo_url)
        self.children = children.Children(self.options)
        self.generations = self.network_git.current_generation

        self.reset_network_wins()

    def idle(self):
        self.children.get_children()

        while self.options.run:
            self.run_generation()

    def run_generation(self):
        print(" directing a tournament")
        self.run()
        print(" finished that tournament")
        print(" storing performances and evolving")
        self.store_performances()
        self.evolve_networks()

        self.network_git.update_remote()
        self.network_git.update_config()
        self.options.check_run()
        print("run is {}".format(self.options.run))
        print(" looping")

        self.generations += 1

    def run(self):
        ids = range(self.options.network_count)
        matchups = self.get_matchups(ids)

        results = self.children.run(matchups)

        for game in results:
            winner = game['winner']
            if winner is None:
                continue

            self.networks_wins[winner] += 1

        self.wins = list_to_str(self.networks_wins)
        print("performance string is {}".format(self.wins))

        log_str = "session gen {}: {} \n".format(self.generations, self.wins)
        with open("tournements.log", "a") as f:
            f.write(json.dumps(results, indent=4))
            f.write(log_str)

        self.reset_network_wins()

    def reset_network_wins(self):
        self.networks_wins = [
            0 for _ in range(self.options.network_count)
        ]

    def get_matchups(self, ids):
        matchups = []
        for id in ids:
            opponent_ids = sample(ids, k=3)

            network_matchups = [
                (self.options.checkers_game, id, opponent_id)
                for opponent_id in opponent_ids
            ]

            matchups += network_matchups

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
    ensure_generation_cfg(director.options.networks_config)
    start = time.time()
    director.idle()
    end = time.time()
    print("total runtime time was {}".format(end - start))
