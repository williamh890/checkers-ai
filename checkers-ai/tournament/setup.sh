#!/bin/bash

cd ../
g++ -std=c++14 networkManagement.cpp utils.cpp network.cpp seeder.cpp network-file-reader.cpp network-file-writer.cpp -lstdc++fs
make
## compile a script for managing single game here
