CMP=g++ -std=c++14 -Wall

GENERATOR=move-generator
CONVERTER=json-to-stl
GAME=checkers-game
MAIN=main

OUT_FILE=checkers.out

OBJ_FILES=$(GENERATOR).o $(CONVERTER).o $(MAIN).o $(GAME).o

all: $(OUT_FILE)

$(OUT_FILE): $(OBJ_FILES)
	$(CMP) -o $(OUT_FILE) $(OBJ_FILES)

$(MAIN).o: $(MAIN).cpp
	$(CMP) -o $(MAIN).o -c $(MAIN).cpp

$(GENERATOR).o: $(GENERATOR).cpp
	$(CMP) -o $(GENERATOR).o -c $(GENERATOR).cpp

$(CONVERTER).o: $(CONVERTER).cpp
	$(CMP) -o $(CONVERTER).o -c $(CONVERTER).cpp

$(GAME).o: $(GAME).cpp
	$(CMP) -o $(GAME).o -c $(GAME).cpp

install:
	curl https://raw.githubusercontent.com/nlohmann/json/develop/src/json.hpp > headers/json.hpp

run: all
	./$(OUT_FILE)

clean:
	rm $(OBJ_FILES) *.out
