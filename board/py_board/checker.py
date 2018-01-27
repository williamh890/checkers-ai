# checker.py
# Author: Hal DiMarchi
# file for checker class

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
