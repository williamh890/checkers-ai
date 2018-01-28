CMP=g++ -std=c++14 -Wall
OFILES=move-generator.o json-to-stl.o

all: move-generator.out

move-generator.out: $(OFILES)
	$(CMP) -o move-generator.out $(OFILES)

move-generator.o: move-generator.cpp
	$(CMP) -o move-generator.o -c move-generator.cpp

json-to-stl.o: json-to-stl.cpp
	$(CMP) -o json-to-stl.o -c json-to-stl.cpp

clean:
	rm $(OFILES) move-generator.out
