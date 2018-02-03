from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.memory cimport shared_ptr
from libcpp.pair cimport pair

from cython.operator cimport dereference as deref


cdef extern from "headers/json.hpp":
  pass

cdef extern from "headers/consts.h" namespace "ai":
  pass

cdef extern from "headers/player.h" namespace "ai":
  cdef cppclass Player:
    Player()

cdef extern from "headers/board.h" namespace "ai":
  pass
  # cdef enum Action:
  #  pass

  #cdef cppclass Board:
  #  pass
  #  Board()
  #  void addPiecesFor(const shared_ptr[Player] & player)
  #  char at(const Position & pos) const
#
  #  string toString()

#  Board getBoard()

cdef extern from "headers/models.h" namespace "ai":
  pass
#  cdef struct Jump:
  #  pass
  #cdef struct Position:
  #  pass
  #cdef struct Piece:
  #  pass

cdef extern from "headers/utils.h" namespace "ai":
  pass

cdef extern from "headers/table-types.h" namespace "ai":
  pass
  # ctypedef vector[vector[int]] MoveTableType
  # ctypedef vector[vector[Jump]] JumpTableType

cdef extern from "headers/move-generator.h" namespace "ai":
  pass
  #cdef cppclass MoveGenerator:
    #MoveGenerator(const MoveTableType & moves, const JumpTableType & jumps) except +
    #vector[int] getMoves(int space)
    #vector[Jump] getJumps(int space)

cdef extern from "headers/json-to-stl.h" namespace "ai":
  pass

cdef extern from "headers/checkers-game.h" namespace "ai":
  cdef cppclass CheckersGame:
    CheckersGame() except +
    CheckersGame(Board & board,
                shared_ptr[Player] red,
                shared_ptr[Player] black) except +
    bool isInvalid(const pair[int, int] & move)
    bool isInvalid(const pair[int, Jump] & jump)
    vector[char] getBoard()
    const char getActivePlayerColor()
    void swapPlayers()
    void printMovesForColor(const string & color)
    void printJumpsForColor(const string & color)

  CheckersGame getCheckersGame() except +
