cdef extern from 'move_generator.pyx' namespace "ai":
  cdef cppclass MoveGenerator:
    MoveGenerator(const MoveTableType &, const MoveTableType &,
                  const JumpTableType &, const JumpTableType &) except +
