vm: main.o vm.o
	g++ -O3 -g -o vm main.o vm.o

main.o: main.cpp
	g++ -O3 -g -o main.o -c main.cpp

vm.o: vm.cpp vm.h
	g++ -O3 -g -o vm.o -c vm.cpp

clean:
	rm -f *.o
	rm -f vm
