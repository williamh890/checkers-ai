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
    echo building gui
    g++ -I/usr/include/python$1m                                     \
        -Iheaders                                                    \
        -o gui-checkers.out                                          \
        $(cat gui-cpp-files.txt)                                     \
        -lpython$1m -std=c++14 -fpermissive -fopenmp
fi
