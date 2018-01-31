CMP=g++ -std=c++14 -Wall

GENERATOR=move-generator
CONVERTER=json-to-stl
GAME=checkers-game
PLAYER=player
UTILS=utils
BOARD=board
MAIN=main

OBJ_FILES=$(GENERATOR).o $(CONVERTER).o $(GAME).o $(PLAYER).o $(UTILS).o $(BOARD).o
OBJ_FILES_WITH_MAIN=$(OBJ_FILES) $(MAIN).o

OUT_FILE=checkers.out

all: $(OUT_FILE)

$(OUT_FILE): $(OBJ_FILES_WITH_MAIN)
	$(CMP) -o $(OUT_FILE) $(OBJ_FILES_WITH_MAIN)

$(MAIN).o: $(MAIN).cpp
	$(CMP) -o $(MAIN).o -c $(MAIN).cpp

$(GENERATOR).o: $(GENERATOR).cpp
	$(CMP) -o $(GENERATOR).o -c $(GENERATOR).cpp

$(CONVERTER).o: $(CONVERTER).cpp
	$(CMP) -o $(CONVERTER).o -c $(CONVERTER).cpp

$(GAME).o: $(GAME).cpp
	$(CMP) -o $(GAME).o -c $(GAME).cpp

$(PLAYER).o: $(PLAYER).cpp
	$(CMP) -o $(PLAYER).o -c $(PLAYER).cpp

$(BOARD).o: $(BOARD).cpp
	$(CMP) -o $(BOARD).o -c $(BOARD).cpp

$(UTILS).o: $(UTILS).cpp
	$(CMP) -o $(UTILS).o -c $(UTILS).cpp

# For making and running tests

TESTING_DIR=tests
TEST_MAIN=$(TESTING_DIR)/test-main
TEST_JSON_TO_STL=$(TESTING_DIR)/test-json-to-stl
TEST_PLAYER=$(TESTING_DIR)/test-player

TEST_OUT_FILE=$(TESTING_DIR)/tests.out

TESTING_OBJ_FILES=$(TEST_MAIN).o $(TEST_JSON_TO_STL).o $(TEST_PLAYER).o $(OBJ_FILES)

tests: $(TEST_OUT_FILE)

$(TEST_OUT_FILE): $(TESTING_OBJ_FILES)
	$(CMP) -o $(TEST_OUT_FILE) $(TESTING_OBJ_FILES)

$(TEST_MAIN).o: $(TEST_MAIN).cpp
	$(CMP) -o $(TEST_MAIN).o -c $(TEST_MAIN).cpp

$(TEST_JSON_TO_STL).o: $(TEST_JSON_TO_STL).cpp
	$(CMP) -o $(TEST_JSON_TO_STL).o -c $(TEST_JSON_TO_STL).cpp

$(TEST_PLAYER).o: $(TEST_PLAYER).cpp
	$(CMP) -o $(TEST_PLAYER).o -c $(TEST_PLAYER).cpp

tests-run: tests
	./$(TEST_OUT_FILE)

run: all
	./$(OUT_FILE)

install:
	curl https://raw.githubusercontent.com/nlohmann/json/develop/src/json.hpp > headers/json.hpp
	curl https://github.com/catchorg/Catch2/releases/download/v2.1.1/catch.hpp > tests/catch.hpp -L

clean:
	rm $(OBJ_FILES) *.out

clean-tests:
	rm $(TESTING_OBJ_FILES)
