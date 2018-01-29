CMP=g++ -std=c++14 -Wall

GENERATOR=move-generator
CONVERTER=json-to-stl
OUT_FILE=checkers.out

OBJ_FILES=$(GENERATOR).o $(CONVERTER).o

all: $(OUT_FILE)

$(OUT_FILE): $(OBJ_FILES)
	$(CMP) -o $(OUT_FILE) $(OBJ_FILES)

$(GENERATOR).o: $(GENERATOR).cpp
	$(CMP) -o $(GENERATOR).o -c $(GENERATOR).cpp

$(CONVERTER).o: $(CONVERTER).cpp
	$(CMP) -o $(CONVERTER).o -c $(CONVERTER).cpp

install:
	curl https://raw.githubusercontent.com/nlohmann/json/develop/src/json.hpp > headers/json.hpp

clean:
	rm $(OBJ_FILES) $(OUT_FILE)
