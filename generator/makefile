CMP=g++ -std=c++14 -Wall
OFILES=move_generator.o

all: move_generator.out

move_generator.out: $(OFILES)
	$(CMP) -o move_generator.out $(OFILES)

move_generator.o: move_generator.cpp
	$(CMP) -o move_generator.o -c move_generator.cpp

clean:
	rm $(OFILES) move_generator.out
