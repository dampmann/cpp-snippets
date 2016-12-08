BIN=macos-bin
all: even odd strip-comments
odd: odd.o 
	g++ -std=c++14 -Wall -o ${BIN}/odd build/odd.o
odd.o:
	g++ -std=c++14 -Wall -c odd.cpp -o build/odd.o
even: even.o 
	g++ -std=c++14 -Wall -o ${BIN}/even build/even.o
even.o:
	g++ -std=c++14 -Wall -c even.cpp -o build/even.o
strip-comments: strip-comments.o 
	g++ -std=c++14 -Wall -o ${BIN}/strip-comments build/strip-comments.o
strip-comments.o:
	g++ -std=c++14 -Wall -c strip-comments.cpp -o build/strip-comments.o
clean:
	rm build/*.o
