CMP=g++ -std=c++14 -Wall
OFILES=move_generator.o

all: move_generator

move_generator: $(OFILES)
	$(CMP) -o move_generator $(OFILES)

move_generator.o: move_generator.cpp
	$(CMP) -o move_generator.o -c move_generator.cpp

clean:
	rm $(OFILES) move_generator
