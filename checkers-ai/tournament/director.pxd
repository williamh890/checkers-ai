# c++ declarations for cython
# director

from libcpp.vector cimport vector
from libcpp.memory cimport shared_ptr
from libcpp.pair cimport pair
from libcpp cimport bool
cimport cython

cdef extern from "../headers/consts.h" namespace "ai":
    pass

cdef extern from "../headers/seeder.h" namespace "ai":
    cdef cppclass Seeder:
        Seeder() except +

cdef extern from "../headers/models.h" namespace "ai":
    struct Piece:
        Piece(char color, int space) except +

    struct Jump:
        Jump(int to, int through) except +
        int to;
        int through;

cdef extern from "../headers/utils.h" namespace "ai":
    pass

cdef extern from "../headers/network.h" namespace "ai":
    cdef cppclass Network:
        Network() except +
        Network(unsigned int id) except +
        void evolve() except +

    void setupNetworks(vector[unsigned int] dimensions,
                       int numberOfNetworks);


cdef extern from "../headers/network-file-io.h" namespace "ai":
    pass

cdef extern from "../headers/player.h" namespace "ai":
    cdef cppclass Player:
      Player()

cdef extern from "../headers/board.h" namespace "ai":
    cdef cppclass Board:
      Board() except +

    Board getBoard()

cdef extern from "../headers/checkers-game.h" namespace "ai":
    ctypedef pair[int, int] MovePackage
    ctypedef pair[int, Jump] JumpPackage
    ctypedef shared_ptr[Player] PlayerPtr
    ctypedef shared_ptr[Seeder] SeederPtr

    cdef cppclass CheckersGame:
        CheckersGame() except +
        CheckersGame(Board & board,
                    shared_ptr[Player] red,
                    shared_ptr[Player] black,
                    SeederPtr seed) except +
        CheckersGame getCheckersGame() except +


cdef extern from "../headers/minimax.h" namespace "ai":
    ctypedef vector[Piece] Pieces;
    ctypedef vector[char] BoardState;
    int minimax(MovePackage & move,
                int depth, char maximizingPlayer,
                CheckersGame & game)
    int minimax(JumpPackage & jump,
                int depth, char maximizingPlayer,
                CheckersGame & game)

    struct GameState:
        GameState(BoardState & board,
                  Pieces & red,
                  Pieces & black,
                  char activePlayerColor)

    cdef cppclass MiniMaxHelper:
        MiniMaxHelper(char maximizingPlayer, CheckersGame game)
