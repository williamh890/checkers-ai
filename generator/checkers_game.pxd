from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.memory cimport shared_ptr
from libcpp.pair cimport pair
from libcpp cimport bool
from cython.operator cimport dereference as deref
cimport cython


cdef extern from "headers/json.hpp":
  pass

cdef extern from "headers/consts.h" namespace "ai":
  pass

cdef extern from "headers/seeder.h" namespace "ai":
  cdef cppclass Seeder:
    Seeder()
  cdef cppclass RandomDeviceSeeder(Seeder):
    RandomDeviceSeeder()
  cdef cppclass SRandSeeder(Seeder):
    SRandSeeder()

  Seeder getSeeder()


cdef extern from "headers/player.h" namespace "ai":
  cdef cppclass Player:
    Player()

cdef extern from "headers/board.h" namespace "ai":
  cdef cppclass Board:
    Board() except +

  Board getBoard()

cdef extern from "headers/models.h" namespace "ai":
  struct Jump:
    Jump(int to, int through) except +
    int to;
    int through;

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

cdef extern from "headers/seeder.h" namespace "ai":
  pass

cdef extern from "headers/network.h" namespace "ai":
  pass

cdef extern from "headers/checkers-game.h" namespace "ai":
  ctypedef pair[int, int] MovePackage
  ctypedef pair[int, Jump] JumpPackage
  ctypedef shared_ptr[Player] PlayerPtr;
  ctypedef shared_ptr[Seeder] SeederPtr;

  cdef cppclass CheckersGame:
    CheckersGame() except +
    CheckersGame(Board & board,
                shared_ptr[Player] red,
                shared_ptr[Player] black,
                shared_ptr[Seeder] seeder) except +
    bool isInvalid(const pair[int, int] & move)
    bool isInvalid(const pair[int, Jump] & jump)
    vector[char] getBoard()
    bool areJumps()
    void makeMove(MovePackage & move)
    void makeJump(JumpPackage & jump)
  CheckersGame getCheckersGame() except +
