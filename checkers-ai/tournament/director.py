# manager of tournament
import options
import children
from random import sample
NETWORK_COUNT = 100


class Director:
    def __init__(self):
        self.options = options.Options()
        self.children = children.Children(self.options)
        self.networks = [x for x in range(NETWORK_COUNT)]

    def run(self):
        for id in range(1):
            opponent_ids = sample(range(NETWORK_COUNT), k=3)
            wins = self.children.run(id, opponent_ids)
            self.networks[id]


if __name__ == "__main__":
    director = Director()
    director.run()
    print(director.options.checkers_game)
