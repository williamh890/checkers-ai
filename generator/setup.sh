#!/bin/bash

if [ -z "$1" ]; then
    echo "setup.sh [py-version]"
    echo ""
    echo "    py-version       Pass the python version number to use for linking."
    echo "                     This will look in /usr/include/python[py-version] for"
    echo "                     Python.h"
    echo ""
else
    python3 setup.py build_ext --inplace

    cython3 --embed checkers_game.pyx -o checkers_game_obj.cpp -3 --cplus -f

    echo looking for python in /usr/include/python$1m
    g++ -I/usr/include/python$1m \
        -o checkers_game_obj      \
        checkers_game_obj.cpp json-to-stl.cpp move-generator.cpp checkers-game.cpp board.cpp utils.cpp player.cpp \
        -lpython3.6m -std=c++14
fi

