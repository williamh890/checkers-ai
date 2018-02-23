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
        Seeder()
    cdef cppclass RandomDeviceSeeder(Seeder):
        RandomDeviceSeeder()
    cdef cppclass SRandSeeder(Seeder):
        SRandSeeder()

    Seeder getSeeder()

cdef extern from "../headers/models.h" namespace "ai":
    pass

cdef extern from "../headers/utils.h" namespace "ai":
    pass
