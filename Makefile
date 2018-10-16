CXX := g++
CXXFLAGS := -Wall -O3 -fno-strict-aliasing -g

all: vm tests
	$(info Done! Quick commands:)
	$(info - Interpret file: ./vm mybinary.bin)
	$(info - Run tests: ./tests)
	$(info - Assemble a file: python3 assembler/assembler.py mycode.asm)

vm: main.o vm.o
	$(CXX) $(CXXFLAGS) -o vm src/main.o src/vm.o

main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -o src/main.o -c src/main.cpp

vm.o: src/vm.cpp src/vm.h
	$(CXX) $(CXXFLAGS) -o src/vm.o -c src/vm.cpp

tests: test.o test_arithmetic.o test.o vm.o
	$(CXX) -o tests test/test.o test/test_arithmetic.o src/vm.o

test.o: test/test.cpp
	$(CXX) -o test/test.o -c test/test.cpp

test_arithmetic.o: test/test_arithmetic.cpp
	$(CXX) -o test/test_arithmetic.o -c test/test_arithmetic.cpp

clean:
	rm -f src/*.o
	rm -f test/*.o
	rm -f vm
	rm -f tests
