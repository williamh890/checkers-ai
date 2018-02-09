# checkers_game.pyx
# Author: Hal Dimarchi
# wrapper of board and gui

import tkinter as tk
import tkinter.ttk as ttk
from time import sleep
cimport checkers_game
from libcpp.pair cimport pair
from libcpp.vector cimport vector
from cython.operator cimport dereference as deref
# from gui import Board_Gui, example_board

BOARD_SIZE = 8
PLAY_SIZE = 4
WINDOW_SIZE = 400
PIECES = {"RED": "R", "BLACK": "B"}

ctypedef pair[int, int] move_type
ctypedef pair[int, Jump] jump_type

cdef class PyCheckersGame:
  cdef CheckersGame checkers_game
  def __cinit__(self):
    self.checkers_game = getSeedlessCheckersGame()

  def get_board(self):
    board = self.checkers_game.getBoard()
    char_board = []
    for space in board:
      char_board.append(chr(space))
    return char_board

  def is_jump_invalid(self, int start, int to, int through):
    cdef Jump jump
    jump.to, jump.through = to, through
    cdef jump_type full_jump = jump_type(start, jump)
    return self.checkers_game.isInvalid(full_jump)

  def is_move_invalid(self, int start, int to):
    cdef move_type move = move_type(start, to)
    return self.checkers_game.isInvalid(move)

  def are_jumps(self):
    return self.checkers_game.areJumps()

  def make_move(self, int start, int to):
    cdef move_type move = move_type(start, to)
    self.checkers_game.makeMove(move)

  def make_jump(self, int start, int to, int through):
    cdef Jump jump
    jump.to, jump.through = to, through
    cdef jump_type full_jump = jump_type(start, jump)
    self.checkers_game.makeJump(full_jump)


class PyBoard():
    def __init__(self):
        self.game = PyCheckersGame()
        self.board = self.game.get_board()
        self.window = tk.Tk()
        self.entry = ttk.Entry(self.window)
        self.entry.bind('<Command-a>', self.get_entry_move)
        self.entry.pack(side=tk.RIGHT)

        self.board = tk.Frame(
            self.window,
            width=WINDOW_SIZE,
            height=WINDOW_SIZE
        )

        self.spaces = [[0 for x in range(BOARD_SIZE)]
                       for y in range(BOARD_SIZE)]
        self.draw_spaces = self.spaces

        self.move_buttons = []
        self.mb_info = []

        self.board.pack()

    def get_board(self, board):  # board should be a 2D array
        one_to_two_dimensions(board, self.spaces, BOARD_SIZE)

    def make_board(self):
        for row in range(BOARD_SIZE):
            for column in range(BOARD_SIZE):
                self.make_board_space(row, column)

########### GUI FUNCTIONS ###########

#********** GUI SETUP **********#
    def run(self):
      while True:
        board = self.game.get_board()
        self.compare_and_update_board(board)
        self.window.update_idletasks()
        self.window.update()

    def make_board_space(self, row, column):
        callback = self.make_move_callback(row, column)
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

    def make_move_callback(self, row, col):
        return lambda: self.move_callback(row, col)

#********** GUI MOVE FUNCTIONS **********#
    def move_callback(self, row, col):
        print('({}, {})'.format(row, col))

        self.move_buttons.append(self.draw_spaces[row][col])
        self.mb_info.append((row, col))

        if len(self.move_buttons) == 2:
          self.b_space = self.move_buttons[0]
          self.e_space = self.move_buttons[1]
          if self.is_diagonal():
            if self.game.are_jumps():
              print("making jump")
              self.submit_jump()
            else:
                print("making move")
                self.submit_move()
            print("select your next move")

    def submit_move(self):
        move = convert_spaces_to_indices(self.mb_info)
        if self.game.is_move_invalid(move[0], move[1]):
          print("bad move")
        else:
          self.b_space["text"], self.e_space["text"] = self.e_space["text"], self.b_space["text"]
          self.game.make_move(move[0], move[1])
          print("buttons should be different")
        self.move_buttons, self.mb_info = [], []

    def submit_jump(self):
      mid_space = (min(self.mb_info[0][0],self.mb_info[1][0]) + 1,
                   min(self.mb_info[0][1], self.mb_info[1][1]) + 1)
      self.mb_info.append(mid_space)
      mid_space = self.draw_spaces[mid_space[0]][mid_space[1]]

      jump = convert_spaces_to_indices(self.mb_info)
      if self.game.is_jump_invalid(jump[0], jump[1], jump[2]):
        print("bad jump")
      else:
        self.e_space["text"] = self.b_space["text"]
        self.b_space["text"] = " "
        mid_space["text"] = " "
        self.game.make_jump(jump[0], jump[1], jump[2])
      self.move_buttons, self.mb_info = [], []

    def is_diagonal(self):
      if self.mb_info[0][0] == self.mb_info[1][0]:
        print("not in play space")
        self.move_buttons, self.mb_info = [], []
        return False
      if self.mb_info[0][1] == self.mb_info[1][1]:
        print("not in play space")
        self.move_buttons, self.mb_info = [], []
        return False
      return True

    def compare_and_update_board(self, board):
      if (board != self.board):
        self.board = board
        self.update_buttons()
        print(board)

    def update_buttons(self):
      for row in range(BOARD_SIZE):
        for col in range(BOARD_SIZE):
          if check_space(row, col):
            index = convert_row_col_to_index((row, col))
            if self.board[index] != self.draw_spaces[row][col]["text"]:
              self.draw_spaces[row][col]["text"] = self.board[index]

    def get_entry_move(self, e):
        print(self.entry.get())

def one_to_two_dimensions(one_board, two_board, dimension):
  for row in range(dimension):
    for col in range(dimension):
      if check_space(row, col):
        two_board[row][col] = one_board[convert_row_col_to_index((row, col))]
      else:
        two_board[row][col] = " "

def check_space(row, column):
  if row % 2 == 0 and column % 2 != 0:
    return True
  elif row % 2 != 0 and column % 2 == 0:
    return True
  return False


def convert_spaces_to_indices(spaces):
  space_nums = []
  for space in spaces:
    space_nums.append(convert_row_col_to_index(space))
  return space_nums

def convert_row_col_to_index(row_col):
  index = ((row_col[0]) * BOARD_SIZE + row_col[1])//2
  return index

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
                        board[row][column] = "b"
                    else:
                        board[row][column] = "r"
                elif row % 2 != 0 and column % 2 == 0:
                    if row <= 2:
                        board[row][column] = "b"
                    else:
                        board[row][column] = "r"
                else:
                    board[row][column] = " "
    return board


if __name__ == "__main__":

    board = example_board()
    checkers = PyBoard()
    checkers.get_board(checkers.game.get_board())
    checkers.make_board()
    checkers.run()
