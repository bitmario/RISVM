CXX ?= g++
CXXFLAGS := -std=c++11 -Wall -O3 -fno-strict-aliasing -g
CXXFLAGS_TEST = -std=c++11 -fno-strict-aliasing 

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

tests: vm.o test.o test_system.o test_registers.o test_stack.o test_memory.o test_arithmetic.o test_conversions.o test_branching.o
	$(CXX) $(CXXFLAGS_TEST) -o tests src/vm.o test/test.o test/test_system.o test/test_registers.o test/test_stack.o test/test_memory.o test/test_arithmetic.o test/test_conversions.o test/test_branching.o

test.o: test/test.cpp
	$(CXX) $(CXXFLAGS_TEST) -o test/test.o -c test/test.cpp

test_system.o: test/test_system.cpp
	$(CXX) $(CXXFLAGS_TEST) -o test/test_system.o -c test/test_system.cpp

test_registers.o: test/test_registers.cpp
	$(CXX) $(CXXFLAGS_TEST) -o test/test_registers.o -c test/test_registers.cpp

test_stack.o: test/test_stack.cpp
	$(CXX) $(CXXFLAGS_TEST) -o test/test_stack.o -c test/test_stack.cpp

test_memory.o: test/test_memory.cpp
	$(CXX) $(CXXFLAGS_TEST) -o test/test_memory.o -c test/test_memory.cpp

test_arithmetic.o: test/test_arithmetic.cpp
	$(CXX) $(CXXFLAGS_TEST) -o test/test_arithmetic.o -c test/test_arithmetic.cpp

test_conversions.o: test/test_conversions.cpp
	$(CXX) $(CXXFLAGS_TEST) -o test/test_conversions.o -c test/test_conversions.cpp

test_branching.o: test/test_branching.cpp
	$(CXX) $(CXXFLAGS_TEST) -o test/test_branching.o -c test/test_branching.cpp

clean:
	rm -f src/*.o
	rm -f test/*.o
	rm -f vm
	rm -f tests
