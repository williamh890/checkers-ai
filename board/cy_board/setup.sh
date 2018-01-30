#!/bin/bash

python3 setup.py build_ext --inplace
cython3 --embed board.pyx -o board.c
gcc -I/usr/include/python3.5m -o board board.c -lpython3.5m
# draws board
./board
