# checkers-game.pyx
# Author: Hal Dimarchi
# .pyx file for checkers game
"""
include "headers/checkers-game.h"
include "headers/json-to-stl.h"
include "headers/models.h"
"""

from libcpp.vector cimport vector
from cython.operator cimport dereference as deref

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
    getMoves(int space)
    getJumps(int space)

cdef extern from "headers/json-to-stl.h" namespace "ai":
  pass

cdef extern from "headers/checkers-game.h" namespace "ai":
  cdef cppclass CheckersGame:
    CheckersGame() except +
    CheckersGame(MoveGenerator, MoveGenerator) except +
    void printBoard()
    void printMoves()

  CheckersGame getCheckersGame() except +

cdef class PyCheckersGame:
  cdef CheckersGame checkers_game
  def __cinit__(self):
    self.checkers_game = getCheckersGame()

  def printBoard(self):
    self.checkers_game.printBoard()

  def printMoves(self):
    self.checkers.printMoves()


if __name__ == "__main__":
  game = PyCheckersGame()
  print("Hewwo")
