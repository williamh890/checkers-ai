import tkinter as tk
from move_generator import get_move_generator

BOARD_SIZE = 8
WINDOW_SIZE = 400


class CheckersGui(object):
    def __init__(self):
        self.window = tk.Tk()

        self.board = tk.Frame(
            self.window,
            width=WINDOW_SIZE,
            height=WINDOW_SIZE
        )

        self.spaces = [[0 for x in range(BOARD_SIZE)]
                       for y in range(BOARD_SIZE)]

        self.add_pieces_to_board()
        self.board.pack()

    def run(self):
        self.window.mainloop()

    def add_pieces_to_board(self):
        for row in range(BOARD_SIZE):
            row = self.add_row(row)

    def add_row(self, row):
        for col in range(BOARD_SIZE):
            self.make_board_space(row, col)

    def make_board_space(self, row, col):
        callback = self.make_space_callback(row, col)
        space = tk.Button(self.board, text='e', command=callback)
        space.grid(row=row, column=col)
        self.spaces[row][col] = space

    def make_space_callback(self, row, col):
        return lambda: self.space_callback(row, col)

    def space_callback(self, row, col):
        print('({}, {})'.format(row, col))


def check_table():
    table = get_move_generator('../generator/move-table.json')

    for space in range(32):
        print(table.get_black_moves_at(space))


if __name__ == "__main__":
    check_table()
    gui = CheckersGui()
    gui.run()
