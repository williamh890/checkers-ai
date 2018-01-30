from distutils.core import setup, Extension
from Cython.Build import cythonize

setup(ext_modules=cythonize(Extension(
           "checkers_game.pyx",                 # our Cython source
           sources=["checkers-game.cpp"],  # additional source file(s)
           language="c++",
           extra_compile_args=["-std=c++14"],
          )))
