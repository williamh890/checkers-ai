CXXFLAGS=-std=c++14
CMP=g++ -std=c++14 -Wall -fopenmp -O3

BUILD_DIR=build
GENERATOR=move-generator
CONVERTER=json-to-stl
GAME=checkers-game
PLAYER=player
UTILS=utils
BOARD=board
SEEDER=seeder
NETWORK=network
NETWORK_READER=network-file-reader
NETWORK_WRITER=network-file-writer
SEARCH=search
GAME_STATE=game-state
MAIN=main

GENERATOR_OBJ=$(BUILD_DIR)/$(GENERATOR).o
CONVERTER_OBJ=$(BUILD_DIR)/$(CONVERTER).o
GAME_OBJ=$(BUILD_DIR)/$(GAME).o
PLAYER_OBJ=$(BUILD_DIR)/$(PLAYER).o
UTILS_OBJ=$(BUILD_DIR)/$(UTILS).o
BOARD_OBJ=$(BUILD_DIR)/$(BOARD).o
SEEDER_OBJ=$(BUILD_DIR)/$(SEEDER).o
MAIN_OBJ=$(BUILD_DIR)/$(MAIN).o
NETWORK_OBJ=$(BUILD_DIR)/$(NETWORK).o
NETWORK_READER_OBJ=$(BUILD_DIR)/$(NETWORK_READER).o
NETWORK_WRITER_OBJ=$(BUILD_DIR)/$(NETWORK_WRITER).o
SEARCH_OBJ=$(BUILD_DIR)/$(SEARCH).o
GAME_STATE_OBJ=$(BUILD_DIR)/$(GAME_STATE).o

OBJ_FILES=$(GENERATOR_OBJ) $(CONVERTER_OBJ) $(GAME_OBJ) $(PLAYER_OBJ) $(UTILS_OBJ) \
		  $(BOARD_OBJ) $(SEEDER_OBJ) \
		  $(NETWORK_OBJ) $(NETWORK_READER_OBJ) $(NETWORK_WRITER_OBJ) \
		  $(SEARCH_OBJ) $(GAME_STATE_OBJ)

CPP_FILES=$(GENERATOR).cpp $(CONVERTER).cpp $(GAME).cpp $(PLAYER).cpp \
		  $(UTILS).cpp $(BOARD).cpp $(SEEDER).cpp                     \
		  $(NETWORK).cpp $(NETWORK_WRITER).cpp $(NETWORK_READER).cpp \
		  $(SEARCH).cpp $(GAME_STATE).cpp

OBJ_FILES_WITH_MAIN=$(OBJ_FILES) $(MAIN).o

OUT_FILE=checkers.out

all: $(OUT_FILE)

$(OUT_FILE): $(OBJ_FILES_WITH_MAIN)
	$(CMP) -o $(OUT_FILE) $(OBJ_FILES_WITH_MAIN)

$(MAIN_OBJ): $(MAIN).cpp
	$(CMP) -o $(MAIN_OBJ) -c $(MAIN).cpp

$(GENERATOR_OBJ): $(GENERATOR).cpp
	$(CMP) -o $(GENERATOR_OBJ) -c $(GENERATOR).cpp

$(CONVERTER_OBJ): $(CONVERTER).cpp
	$(CMP) -o $(CONVERTER_OBJ) -c $(CONVERTER).cpp

$(GAME_OBJ): $(GAME).cpp
	$(CMP) -o $(GAME_OBJ) -c $(GAME).cpp

$(PLAYER_OBJ): $(PLAYER).cpp
	$(CMP) -o $(PLAYER_OBJ) -c $(PLAYER).cpp

$(BOARD_OBJ): $(BOARD).cpp
	$(CMP) -o $(BOARD_OBJ) -c $(BOARD).cpp

$(UTILS_OBJ): $(UTILS).cpp
	$(CMP) -o $(UTILS_OBJ) -c $(UTILS).cpp

$(SEEDER_OBJ): $(SEEDER).cpp
	$(CMP) -o $(SEEDER_OBJ) -c $(SEEDER).cpp

$(NETWORK_OBJ): $(NETWORK).cpp
	$(CMP) -o $(NETWORK_OBJ) -c $(NETWORK).cpp

$(NETWORK_READER_OBJ): $(NETWORK_READER).cpp
	$(CMP) -o $(NETWORK_READER_OBJ) -c $(NETWORK_READER).cpp

$(NETWORK_WRITER_OBJ): $(NETWORK_WRITER).cpp
	$(CMP) -o $(NETWORK_WRITER_OBJ) -c $(NETWORK_WRITER).cpp

$(SEARCH_OBJ): $(SEARCH).cpp
	$(CMP) -o $(SEARCH_OBJ) -c $(SEARCH).cpp

$(GAME_STATE_OBJ): $(GAME_STATE).cpp
	$(CMP) -o $(GAME_STATE_OBJ) -c $(GAME_STATE).cpp


manager:
	g++ -std=c++14 -o manager.out 	\
		networkManagement.cpp 		\
		utils.cpp 					\
		network.cpp 				\
		seeder.cpp 					\
		network-file-reader.cpp 	\
		network-file-writer.cpp 	\
		-lstdc++fs
# For making and running tests

TESTING_DIR=tests
TEST_MAIN=$(TESTING_DIR)/test-main
TEST_JSON_TO_STL=$(TESTING_DIR)/test-json-to-stl
TEST_PLAYER=$(TESTING_DIR)/test-player
TEST_UTILS=$(TESTING_DIR)/test-utils
TEST_MOVE_GENERATOR=$(TESTING_DIR)/test-move-generator
TEST_NETWORK=$(TESTING_DIR)/test-network
TEST_SEARCH=$(TESTING_DIR)/test-search

TEST_OUT_FILE=$(TESTING_DIR)/tests.out

TESTING_OBJ_FILES=$(TEST_MAIN).o $(TEST_JSON_TO_STL).o $(TEST_PLAYER).o \
				  $(TEST_UTILS).o $(TEST_MOVE_GENERATOR).o $(TEST_NETWORK).o \
				   $(TEST_SEARCH).o \
				  $(OBJ_FILES)

tests: $(TEST_OUT_FILE)

$(TEST_OUT_FILE): $(TESTING_OBJ_FILES)
	$(CMP) -o $(TEST_OUT_FILE) $(TESTING_OBJ_FILES)

$(TEST_MAIN).o: $(TEST_MAIN).cpp
	$(CMP) -o $(TEST_MAIN).o -c $(TEST_MAIN).cpp

$(TEST_JSON_TO_STL).o: $(TEST_JSON_TO_STL).cpp
	$(CMP) -o $(TEST_JSON_TO_STL).o -c $(TEST_JSON_TO_STL).cpp

$(TEST_PLAYER).o: $(TEST_PLAYER).cpp
	$(CMP) -o $(TEST_PLAYER).o -c $(TEST_PLAYER).cpp

$(TEST_UTILS).o: $(TEST_UTILS).cpp
	$(CMP) -o $(TEST_UTILS).o -c $(TEST_UTILS).cpp

$(TEST_MOVE_GENERATOR).o: $(TEST_MOVE_GENERATOR).cpp
	$(CMP) -o $(TEST_MOVE_GENERATOR).o -c $(TEST_MOVE_GENERATOR).cpp

$(TEST_NETWORK).o: $(TEST_NETWORK).cpp
	$(CMP) -o $(TEST_NETWORK).o -c $(TEST_NETWORK).cpp

$(TEST_SEARCH).o: $(TEST_SEARCH).cpp
	$(CMP) -o $(TEST_SEARCH).o -c $(TEST_SEARCH).cpp

tests-run: tests
	mkdir -p logs
	./$(TEST_OUT_FILE) $1

run: all
	./$(OUT_FILE)

install:
	mkdir -p $(BUILD_DIR)
	curl https://github.com/nlohmann/json/releases/download/v3.1.0/json.hpp > headers/json.hpp -L
	curl https://github.com/catchorg/Catch2/releases/download/v2.1.1/catch.hpp > tests/catch.hpp -L

VERSION=3.5.5
PYTHON_SRC_DIR=Python-$(VERSION)
PYDIR=python

clean:
	rm $(OBJ_FILES) $(MAIN).o *.out

clean-tests:
	rm $(TESTING_OBJ_FILES)
