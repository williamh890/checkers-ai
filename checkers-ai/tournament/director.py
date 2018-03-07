# manager of tournament
import options
import children


class Director:
    def __init__(self):
        self.options = options.Options()
        self.children = children.Children(self.options)

    def run(self):
        self.children.run()


if __name__ == "__main__":
    director = Director()
    director.run()
    print(director.options.checkers_game)
