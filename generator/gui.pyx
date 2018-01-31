# gui.pyx
# Author: Hal Dimarchi
# board gui

import tkinter as tk
import tkinter.ttk as ttk

BOARD_SIZE = 8
WINDOW_SIZE = 400


class Board_Gui():
    def __init__(self):
        self.window = tk.Tk()

        self.board = tk.Frame(
            self.window,
            width=WINDOW_SIZE,
            height=WINDOW_SIZE
        )

        self.spaces = [[0 for x in range(BOARD_SIZE)]
                       for y in range(BOARD_SIZE)]
        self.draw_spaces = self.spaces

        self.move = []

        self.board.pack()

    def run(self):
        self.window.mainloop()

    def get_board(self, board):  # board should be a 2D array
        self.spaces = board

    def make_board(self):
        for row in range(BOARD_SIZE):
            for column in range(BOARD_SIZE):
                self.make_board_space(row, column)

    def make_board_space(self, row, column):
        callback = self.make_space_callback(row, column)
        space = tk.Button(
                          self.board,
                          text=str(self.spaces[row][column]),
                          command=callback
                    )
        space.grid(row=row, column=column)
        self.draw_spaces[row][column] = space

    def make_space_callback(self, row, col):
        return lambda: self.space_callback(row, col)

    def space_callback(self, row, col):
        print('({}, {})'.format(row, col))

    def move_callback(self, row, col):
      self.move.push_back((row, col))
      if len(self.move) == 2
        print("making move")
        self.submit_move()
      else:
        print("select your next move")

    def submit_move(self):
      b_row, b_col = *self.move[0]
      e_row, e_col = *self.move[1]
      self.spaces[b_row, b_col],
      self.spaces[e_row, e_col] = self.spaces[e_row, e_col],
                                  self.spaces[b_row, b_col]


def example_board():
  board = [[0 for x in range(BOARD_SIZE)]
           for y in range(BOARD_SIZE)]

  for row in range(BOARD_SIZE):
      for column in range(BOARD_SIZE):
          if row > 2 and row < 5:
              board[row][column] = " "
          else:
              if row % 2 == 0 and column % 2 != 0:
                  if row <= 2:
                      board[row][column] = "B"
                  else:
                      board[row][column] = "R"
              elif row % 2 != 0 and column % 2 == 0:
                  if row <= 2:
                      board[row][column] = "B"
                  else:
                      board[row][column] = "R"
              else:
                  board[row][column] = " "
    return board

if __name__ == "__main__":

    board = example_board()
    board_gui = Board_Gui()
    board_gui.get_board(board)
    board_gui.make_board()
    board_gui.run()
