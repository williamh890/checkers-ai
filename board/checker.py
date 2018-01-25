# checker.py
# Author: Hal DiMarchi
# file for checker class

class Checkers():
    def __init__(self, color, start_space, end_space):
        self.color = color
        self.make_checkers(start_space, end_space)

    def make_checkers(self, start_space, end_space):
        self.checkers = []
        for i in range(start_space, end_space):
            self.checkers.append(Checker(i - start_space, self.color, i))



class Checker():
    def __init__(self, id, color, space):
        self.id = id
        self.color = color
        self.space = space

    def consider_moves(self):
        # move generator stuff
        pass

    def move(self, space):
        self.space = space
