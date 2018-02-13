from distutils.core import setup, Extension
from Cython.Build import cythonize

setup(name="cython_checkers",
      ext_modules=cythonize(Extension(
           "checkers_game.pyx",                 # our Cython source
           sources=["checkers-game.cpp", "move-generator.cpp",
                    "json-to-stl.cpp", "board.cpp", "player.cpp",
                    "utils.cpp", "gui-cpp-interface.cpp", "seeder.cpp",
                    "network.cpp", "network-file-reader.cpp",
                    "network-file-writer.cpp"],  # additional source file(s)
           language="c++",
           extra_compile_args=["-std=c++14"],
          )))