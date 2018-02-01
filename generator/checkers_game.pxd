from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.memory cimport shared_ptr
from cython.operator cimport dereference as deref


cdef extern from "headers/json.hpp":
  pass

cdef extern from "headers/consts.h" namespace "ai":
  pass

cdef extern from "headers/player.h" namespace "ai":
  cdef cppclass Player:
    Player()
    Player(char color, MoveGenerator generator)
    const vector[Piece] getPieces() const
    const char getColor() const

    const vector[int] getMovesFor(Piece piece) const
    const vector[Jump] getJumpsFor(Piece piece) const

cdef extern from "headers/board.h" namespace "ai":
  cdef cppclass Board:
    Board()
    void addPiecesFor(const shared_ptr[Player] & player)
    void print()

  Board getBoard()

cdef extern from "headers/models.h" namespace "ai":
  cdef struct Jump:
    pass
  cdef struct Position:
    pass
  cdef struct Piece:
    pass

cdef extern from "headers/utils.h" namespace "ai":
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
    CheckersGame(Board & board,
                 shared_ptr[Player] red,
                 shared_ptr[Player] black) except +
    void print()
    void printMoves()
    void printMovesForColor(const string & color)
    void printJumpsForColor(const string & color)

  CheckersGame getCheckersGame() except +
