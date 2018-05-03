from libcpp.vector cimport vector
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
                SeederPtr seed) except +
    bool isInvalid(MovePackage & move)
    bool isInvalid(JumpPackage & jump)
    vector[char] getBoard()
    bool areJumps()
    void makeMove(MovePackage & move) except +
    void makeJump(JumpPackage & jump) except +

cdef extern from "headers/get-checkers-game.h" namespace "ai":
  CheckersGame getCheckersGame() except +
  CheckersGame getNetworkedCheckersGame(int red_id, int black_id) except +
  CheckersGame getNetworkVPieceCountCheckersGame(unsigned int network_id, char networked_player) except +

cdef extern from "headers/gui-cpp-interface.h" namespace "ai":
  ctypedef pair[int, int] MovePackage
  ctypedef pair[int, Jump] JumpPackage
  ctypedef shared_ptr[Player] PlayerPtr;
  ctypedef shared_ptr[Seeder] SeederPtr;

  cdef cppclass GuiCppInterface:
    GuiCppInterface() except +
    GuiCppInterface(const CheckersGame & game) except +
    void play() except +
    bool isInvalid(const pair[int, int] & move)
    bool isInvalid(const pair[int, Jump] & jump)
    vector[char] getBoard()
    char getActivePlayerColor()
    char getInactivePlayerColor()
    bool areJumps()
    bool areMoves()
    vector[vector[int]] getGame() except +
    void makeMove(MovePackage & move) except +
    void makeJump(JumpPackage & jump) except +
    void replayJump(JumpPackage & jump) except +
    void replayMove(MovePackage & move) except +
