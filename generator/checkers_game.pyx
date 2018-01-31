# checkers-game.pyx
# Author: Hal Dimarchi
# .pyx file for checkers game
"""
include "headers/checkers-game.h"
include "headers/json-to-stl.h"
include "headers/models.h"
"""

from libcpp.vector cimport vector
from libcpp.string cimport string
from cython.operator cimport dereference as deref
from gui import Board_Gui, example_board

cdef extern from "headers/json.hpp":
  pass

cdef extern from "headers/models.h" namespace "ai":
  cdef struct Jump:
    pass

cdef extern from "headers/table-types.h" namespace "ai":
  ctypedef vector[vector[int]] MoveTableType
  ctypedef vector[vector[Jump]] JumpTableType

cdef extern from "headers/move-generator.h" namespace "ai":
  cdef cppclass MoveGenerator:
    MoveGenerator(const MoveTableType & moves, const JumpTableType & jumps) except +
    vector[int] getMoves(int space)
    vector[Jump] getJumps(int space)

cdef extern from "headers/json-to-stl.h" namespace "ai":
  pass

cdef extern from "headers/checkers-game.h" namespace "ai":
  cdef cppclass CheckersGame:
    CheckersGame() except +
    CheckersGame(MoveGenerator, MoveGenerator) except +
    void printBoard()
    void printMoves()
    void printMovesForColor(const string & color);
    void printJumpsForColor(const string & color);
    # waiting for this to be defined in source:
    # void printValidMoves();

  CheckersGame getCheckersGame() except +

cdef class PyCheckersGame:
  cdef CheckersGame checkers_game
  def __cinit__(self):
    self.checkers_game = getCheckersGame()

  def printBoard(self):
    self.checkers_game.printBoard()

  def printMoves(self):
    self.checkers.printMoves()

  def printMovesForColor(self, color):
    self.checkers_game.printMovesForColor(color)

  def printJumpsForColor(self, color):
    self.checkers_game.printJumpsForColor(color)

  def printValidMoves(self):
    pass
    # waiting for this to be defined in source
    # self.checkers_game.printValidMoves()


if __name__ == "__main__":
  game = PyCheckersGame()
  gui = Board_Gui()
  board = example_board()
  gui.get_board(board)
  gui.make_board()
  gui.run()
  
  game.printBoard()
  print("Hewwo")
