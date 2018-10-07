vm: main.o vm.o
	g++ -g -o vm main.o vm.o

main.o: main.cpp
	g++ -g -o main.o -c main.cpp

vm.o: vm.cpp vm.h
	g++ -g -o vm.o -c vm.cpp

clean:
	rm -f *.o
	rm -f vm
