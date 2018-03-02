    export DISPLAY=:0

    python3 setup.py build_ext --inplace

    cython3 --embed checkers_game.pyx -o checkers_game_obj.cpp -3 --cplus -f


    echo looking for python in /usr/include/python3.6m
    g++ -I/usr/include/python3.6m                                    \
        -o gui-checkers.out                                          \
        checkers_game_obj.cpp json-to-stl.cpp move-generator.cpp     \
        checkers-game.cpp board.cpp utils.cpp player.cpp seeder.cpp  \
        gui-cpp-interface.cpp minimax.cpp                            \
        -lpython3.6m -std=c++14

