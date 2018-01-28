CMP=g++ -std=c++14 -Wall
OFILES=move_generator.o json-to-stl.o

all: move_generator.out

move_generator.out: $(OFILES)
	$(CMP) -o move_generator.out $(OFILES)

move_generator.o: move_generator.cpp
	$(CMP) -o move_generator.o -c move_generator.cpp

json-to-stl.o: json-to-stl.cpp
	$(CMP) -o json-to-stl.o -c json-to-stl.cpp

clean:
	rm $(OFILES) move_generator.out
