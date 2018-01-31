#!/bin/bash

python3 setup.py build_ext --inplace
cython3 --embed checkers_game.pyx -o checkers_game_obj.cpp -3 --cplus -f
g++ -I/usr/include/python3.5m -o checkers_game_obj checkers_game_obj.cpp json-to-stl.cpp move-generator.cpp checkers-game.cpp -lpython3.5m -std=c++14
