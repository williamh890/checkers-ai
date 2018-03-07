#!/bin/bash

cd /home/hal/Documents/school/checkers_ai/checkers-ai/
g++ -std=c++14 -o manager.out networkManagement.cpp utils.cpp network.cpp seeder.cpp network-file-reader.cpp network-file-writer.cpp -lstdc++fs
make
## compile a script for managing single game here
