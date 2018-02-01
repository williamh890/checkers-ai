from libcpp.vector cimport vector
from libcpp.string cimport string
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

  CheckersGame getCheckersGame() except +
