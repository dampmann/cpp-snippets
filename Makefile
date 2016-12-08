BIN=macos-bin
all: even odd
odd: odd.o 
	g++ -std=c++14 -Wall -o ${BIN}/odd build/odd.o
odd.o:
	g++ -std=c++14 -Wall -c odd.cpp -o build/odd.o
even: even.o 
	g++ -std=c++14 -Wall -o ${BIN}/even build/even.o
even.o:
	g++ -std=c++14 -Wall -c even.cpp -o build/even.o
clean:
	rm build/*.o
