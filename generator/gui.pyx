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

    self.board.pack()

  def run(self):
    self.window.mainloop()

  def get_board(self, board): # board should be a 2D array
    self.spaces = board

  def make_board(self):
    for row, col in self.spaces:
      self.make_board_space(row, col)

  def make_board_space(self, row, col):
    callback = self.make_space_callback(row, col)
    space = tk.Button(self.board, text='e', command=callback)
    space.grid(row=row, column=col)
    self.draw_spaces[row][col] = space

  def make_space_callback(self, row, col):
    return lambda: self.space_callback(row, col)

  def space_callback(self, row, col):
    print('({}, {})'.format(row, col))
