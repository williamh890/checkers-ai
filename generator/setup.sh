#!/bin/bash

python3 setup.py build_ext --inplace
#cython3 --embed pycheckers.pyx -o pycheckers.cpp -3 --cplus -f
#g++ -I/usr/include/python3.5m -o pycheckers_obj pycheckers.cpp json-to-stl.cpp move-generator.cpp checkers-game.cpp -lpython3.5m -std=c++14
cython3 --embed checkers_game.pyx -o checkers_game_obj.cpp -3 --cplus -f
g++ -I/usr/include/python3.5m -o checkers_game_obj checkers_game_obj.cpp json-to-stl.cpp move-generator.cpp checkers-game.cpp -lpython3.5m -std=c++14
# g++ -fPIC -I/usr/include/python3.5m checkers_game_obj.o json-to-stl.o move-generator.o checkers-game.o -o checkers_game_obj.so -lpython3.5m
