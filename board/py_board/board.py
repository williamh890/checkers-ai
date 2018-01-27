# board.py
# Authors: Hal DiMarchi
# checkers board class

from move_generator import get_move_generator
import space
import player

BLACK = "black"
RED = "red"

class Board(object):
    def __init__(self):
        self.move_generator = get_move_generator('../../generator/move-table.json')
        self.make_spaces()
        self.make_checkers()
        self.active_player = self.black_player
        self.inactive_player = self.red_player

    def make_spaces(self):
        self.board_spaces = space.Spaces()

    def make_checkers(self):
        self.red_player = player.Player(RED, self.move_generator.red)
        self.black_player = player.Player(BLACK, self.move_generator.black)
        self.board_spaces.place_checkers(self.red_player, self.black_player)

    def turn(self):
        moves = self.active_player.consider_moves()
        self.check_jumps(moves["jumps"])
        self.check_moves(moves["moves"])
        # somewhere in here evaluate for end game condition
        # (all moves lead to last checkers death (cycled state checking too))
        if len(self.jumps)>0:
            # need to update other players checkers
            # need to update new space
            self.active_player.make_jump(self.jumps)
        elif(self.moves)>0:
            self.active_player.make_move(self.moves)
        else:
            # what?
            print("something's wrong, or/and I don't understand checkers")

        swap(self.active_player, self.other_player)



    def check_jumps(self, jumps):
        for checker, spaces in jumps.items():
            if len(spaces)>0:
                for space in spaces:
                    space_checker_color = self.board_spaces[space].get_checker_color()
                    if len(space_checker_color)==0 or self.active_player.is_my_color(space_checker_color):
                        spaces.remove(space)
                if len(spaces) == 0:
                    del moves[checker]

            self.jumps = jumps

    def check_moves(self, moves):
        for checker, spaces in moves.items():
            if len(spaces)>0:
                for space in spaces:
                    space_checker_color = self.board_spaces[space].get_checker_color()
                    if len(space_checker_color)>0:
                        spaces.remove(space)
                if len(spaces) == 0:
                    del moves[checker]

        self.moves = moves

    def draw(self):  # some tklinter stuff, or maybe using separate gui package
        board_str=" "
        for space in self.board_spaces.spaces:
            if space.checker:
                if space.checker.color == RED:
                    board_str = board_str + "R"
                else:
                    board_str = board_str + "B"
            if (space.id+1)%4 == 0:
                if(space.id+1)%8 == 0:
                    board_str = board_str + " \n "
                else:
                    board_str = board_str + "\n"
            else:
                board_str = board_str + " "

        print(board_str)

def swap(x, y):
    x, y = y, x

if __name__ == "__main__":
    game_board = Board()
    game_board.draw()
