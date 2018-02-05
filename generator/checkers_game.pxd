from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.memory cimport shared_ptr
from libcpp.pair cimport pair
from libcpp cimport bool

from cython.operator cimport dereference as deref


cdef extern from "headers/json.hpp":
  pass

cdef extern from "headers/consts.h" namespace "ai":
  pass

cdef extern from "headers/player.h" namespace "ai":
  cdef cppclass Player:
    Player()

cdef extern from "headers/board.h" namespace "ai":
  cdef cppclass Board:
    Board() except +

  Board getBoard()

cdef extern from "headers/models.h" namespace "ai":
  cdef struct Jump:
    Jump(int to, int through) except +

  cdef struct Position:
    pass

  cdef struct Piece:
    pass

cdef extern from "headers/utils.h" namespace "ai":
  pass

cdef extern from "headers/table-types.h" namespace "ai":
  pass

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

    pair[vector[char], U] Turn[U](const pair[int, int] & move)
    pair[vector[char], U] Turn[U](const pair[int, Jump] & jump)

  CheckersGame getCheckersGame() except +
