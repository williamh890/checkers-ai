import json
cimport director

from libcpp.vector cimport vector

INITIAL_PERFORMANCE = 0
ctypedef pair[int, int] move_type
ctypedef pair[int, Jump] jump_type

cdef void py_setup_networks(vector[unsigned int] dimensions,
                            int num_networks):
          setupNetworks(dimensions, num_networks)

cdef int py_move_minimax(int start, int to,
                 int depth, char maximizingPlayer,
                 CheckersGame game):
      cdef move_type move = move_type(start, to)
      result = minimax(move, depth,
                     maximizingPlayer,
                     game)
      return result

cdef int py_jump_minimax(int start, int to, int through,
                       int depth, char maximizingPlayer,
                       CheckersGame game):
      cdef Jump jump
      jump.to, jump.through = to, through
      cdef jump_type full_jump = jump_type(start, jump)
      result = minimax(full_jump, depth,
                     maximizingPlayer,
                     game)
      return result

cdef class PyNetwork:
    cdef Network network
    def __cinit__(self, id):
        self.network = Network(id)
    def evolve_network(self):
        self.network.evolve()


class Director:
    def __init__(self, id_list):
        self.setup_network_dictionary(id_list)
    def setup_network_dictionary(self, id_list):
        self.networks = {}
        for id in id_list:
            self.networks[id] = INITIAL_PERFORMANCE
