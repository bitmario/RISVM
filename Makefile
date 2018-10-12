vm: main.o vm.o
	g++ -Wall -O3 -march=native -fno-strict-aliasing -g -o vm main.o vm.o

main.o: main.cpp
	g++ -Wall -O3 -march=native -fno-strict-aliasing -g -o main.o -c main.cpp

vm.o: vm.cpp vm.h
	g++ -Wall -O3 -march=native -fno-strict-aliasing -g -o vm.o -c vm.cpp

clean:
	rm -f *.o
	rm -f vm
