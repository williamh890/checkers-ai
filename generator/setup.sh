#!/bin/bash

python3 setup.py build_ext --inplace
cython3 --embed checkers_game.pyx -o checkers_game.c
g++ -I/usr/include/python3.5m -o checkers_game_obj checkers_game.c -lpython3.5m -std=c++14
