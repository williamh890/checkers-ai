# checkers_game.pyx
# Author: Hal Dimarchi
# wrapper of board and gui

import json
import random
import string
from helper.conversions import one_to_two_dimensions, convert_spaces_to_indices, convert_row_col_to_index
from helper.checking import is_diagonal, check_space
from helper.writer import write_game
from helper.reader import Reader
from helper.game_parser import GameParser

import tkinter as tk
import tkinter.ttk as ttk
cimport checkers_game
from libcpp.pair cimport pair
from libcpp.vector cimport vector
from cython.operator cimport dereference as deref

BOARD_SIZE = 8
PLAY_SIZE = 4
WINDOW_SIZE = 400
PIECES = {"RED": "R", "BLACK": "B"}

ctypedef pair[int, int] move_type
ctypedef pair[int, Jump] jump_type

cdef class PyCheckersGame:
  cdef GuiCppInterface checkers_game
  cdef int move_counter
  def __cinit__(self):
    self.checkers_game = GuiCppInterface(getNetworkedCheckersGame(0, 1))
    self.move_counter = 0

  def get_board(self):
    board = self.checkers_game.getBoard()
    char_board = []
    for space in board:
      space = chr(space)
      char_board.append(str(space))
    return char_board

  def play(self):
    if self.move_counter < 100:
      self.checkers_game.play()
      self.move_counter += 1
    else:
      raise Exception("Exceed Move Limit")

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

  def are_moves(self):
    return self.checkers_game.areMoves()

  def get_game(self):
    return self.checkers_game.getGame()

  def make_move(self, int start, int to):
    cdef move_type move = move_type(start, to)
    try:
      self.checkers_game.makeMove(move)
    except Exception as e:
      print(type(e))
      print(str(e))

  def make_jump(self, int start, int to, int through):
    cdef Jump jump
    jump.to, jump.through = to, through
    cdef jump_type full_jump = jump_type(start, jump)
    try:
      self.checkers_game.makeJump(full_jump)
    except Exception as e:
      print(type(e))
      print(str(e))

  def replay_jump(self, int start, int through, int to):
    cdef Jump jump
    jump.to, jump.through = to, through
    cdef jump_type full_jump = jump_type(start, jump)
    self.checkers_game.replayJump(full_jump)

  def replay_move(self, int start, int to):
    cdef move_type move = move_type(start, to)
    self.checkers_game.replayMove(move)

  def get_winner(self):
    return chr(self.checkers_game.getInactivePlayerColor())


class PyBoard():
    def __init__(self):
        self.game = PyCheckersGame()
        self.game_board = self.game.get_board()
        self.window = tk.Tk()
        self.entry = ttk.Entry(self.window)
        self.entry.bind('<Command-a>', self.get_entry_move)
        self.entry.pack(side=tk.RIGHT)

        self.reader = Reader()

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

#********** GUI SETUP AND RUNNING **********#
    def run(self):
      while True:
        try:
          board = self.game.get_board()
          self.compare_and_update_board(board)
          self.window.update_idletasks()
          self.window.update()
        except Exception as e:
          print(str(e))
          self.run_end_game()

    def run_without_interface(self):
      while True:
        try:
          self.game.play()
        except Exception as e:
          print(str(e))
          self.run_end_game()
        board = self.game.get_board()
        self.compare_and_update_board(board)
        self.window.update_idletasks()
        self.window.update()

    def run_from_file(self):
      self.reader.get_file("boardfZtuqtCK") # place a board you've generated here
      self.reader.get_game()                # or use get_file_from_command_line
      self.parser = GameParser(self.reader.game)
      while True:
        if self.parser.advance():
          print(self.parser.move)
          self.game.replay_jump(self.parser.move[0],
                                self.parser.move[1],
                                self.parser.move[2])
        elif not self.parser.end_game:
          print(self.parser.move)
          self.game.replay_move(self.parser.move[0], self.parser.move[1])
        else:
          self.run_end_game()

        board = self.game.get_board()
        self.compare_and_update_board(board)
        self.window.update_idletasks()
        self.window.update()

    def run_end_game(self):
      write_game(self.game.get_game())
      print("GAME IS OVER")
      print("{} wins".format(self.game.get_winner()))
      reset = input("Play again? (y/n)")
      if reset == "n":
        exit(0)
      self.game = PyCheckersGame()
      option = input("Select a game type: Computer v. Human('h'), Computer v. Computer('c'), From File('f')")
      if option == 'h':
        self.run()
      if option == 'c':
        self.run_without_interface()
      else:
        self.run_from_file()

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
          if not is_diagonal(self.mb_info):
            self.move_buttons, self.mb_info = [], []
            return
          if self.game.are_jumps():
            print("making jump")
            self.submit_jump()
          elif self.game.are_moves():
              print("making move")
              self.submit_move()
          else:
            self.run_end_game()

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

    def compare_and_update_board(self, board):
      if (board != self.game_board):
        self.game_board = board
        self.update_buttons()
        print(board)

    def update_buttons(self):
      for row in range(BOARD_SIZE):
        for col in range(BOARD_SIZE):
          if check_space(row, col):
            index = convert_row_col_to_index((row, col))
            if self.game_board[index] != self.draw_spaces[row][col]["text"]:
              self.draw_spaces[row][col]["text"] = self.game_board[index]

    def get_entry_move(self, e):
        print(self.entry.get())


if __name__ == "__main__":
    checkers = PyBoard()
    checkers.get_board(checkers.game.get_board())
    checkers.make_board()
    checkers.run_without_interface()
