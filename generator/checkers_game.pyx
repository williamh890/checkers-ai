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
    self.checkers_game = getCheckersGame()

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
        self.spaces = board

    def make_board(self):
        for row in range(BOARD_SIZE):
            for column in range(BOARD_SIZE):
                self.make_board_space(row, column)

########### GUI FUNCTIONS ###########

#********** GUI SETUP **********#
    def run(self):
        self.window.mainloop()

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
          if abs(self.mb_info[0][0] - self.mb_info[1][0]) == 2:
            if abs(self.mb_info[0][1] - self.mb_info[1][1]) == 2:
              print("making jump")
              self.submit_jump()
            else:
              print("making move")
              self.submit_move()
          else:
            print("making move, not close to a jump")
            self.submit_move()

        else:
            print("select your next move")

    def submit_move(self):
        move = convert_row_col_to_number(self.mb_info)
        if self.game.is_move_invalid(move[0], move[1]):
          print(move[0])
          print(move[1])
          print("bad move")
        else:
          self.game.make_move(move[0], move[1])
          self.b_space["text"], self.e_space["text"] = self.e_space["text"], self.b_space["text"]
          print("buttons should be different")
        self.move_buttons, self.mb_info = [], []

    def submit_jump(self):
      mid_space = (min(self.mb_info[0][0],self.mb_info[1][0]) + 1,
                   min(self.mb_info[0][1], self.mb_info[1][1]) + 1)
      print(mid_space[0])
      print(mid_space[1])
      self.mb_info.append(mid_space)
      mid_space = self.draw_spaces[mid_space[0]][mid_space[1]]

      jump = convert_row_col_to_number(self.mb_info)
      if self.game.is_jump_invalid(jump[0], jump[1], jump[2]):
        print("bad jump")
      else:
        self.game.make_jump(jump[0], jump[1], jump[2])
        self.e_space["text"] = self.b_space["text"]
        self.b_space["text"] = " "
        mid_space["text"] = " "
      self.move_buttons, self.mb_info = [], []

    def receive_move(self, b_row, b_col, e_row, e_col):
        self.move_buttons = []
        self.move_buttons.append(self.draw_spaces[b_row][b_col])
        self.move_buttons.append(self.draw_spaces[e_row][e_col])
        self.submit_move()

    def get_entry_move(self, e):
        print(self.entry.get())

def convert_row_col_to_number(spaces):
  space_nums = []
  for space in spaces:
    print(space[0])
    print(space[1])
    number = (space[0]) * BOARD_SIZE + space[1]
    print(number)
    print(number//2)
    space_nums.append(number//2)
  return space_nums

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
    checkers = PyBoard()
    checkers.get_board(board)
    checkers.make_board()
    checkers.run()
