from distutils.core import setup, Extension
from Cython.Build import cythonize

with open("gui-cpp-files.txt", "r") as f:
    sources = f.read().strip().split(" ")

setup(name="cython_checkers",
      ext_modules=cythonize(Extension(
           "checkers_game.pyx",                 # our Cython source
           sources=sources,
           language="c++",
           extra_compile_args=["-std=c++14", "-Iheaders", "-fopenmp"],
      )))
