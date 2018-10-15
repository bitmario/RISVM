CXX := g++
CXXFLAGS := -Wall -O3 -fno-strict-aliasing -g

all: vm

vm: main.o vm.o
	$(CXX) $(CXXFLAGS) -o vm src/main.o src/vm.o

main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -o src/main.o -c src/main.cpp

vm.o: src/vm.cpp src/vm.h
	$(CXX) $(CXXFLAGS) -o src/vm.o -c src/vm.cpp

clean:
	rm -f src/*.o
	rm -f vm
