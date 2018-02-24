from distutils.core import setup, Extension
from Cython.Build import cythonize

setup(name="cython_tournament",
      ext_modules=cythonize(Extension(
           "director.pyx",                 # our Cython source
           sources=["../network.cpp", "../seeder.cpp",
                    "../network-file-reader.cpp", "../network-file-writer.cpp",
                    "../utils.cpp", "../minimax.cpp",
                    "../checkers-game.cpp", "../move-generator.cpp",
                    "../json-to-stl.cpp", "../board.cpp", "../player.cpp"
                    ],
           language="c++",
           extra_compile_args=["-std=c++14", "-O3"],
      )))
