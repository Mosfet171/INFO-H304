# Makefile for proty

all: projet

projet: main.o Algo.o Blosum.o file.o Phr.o Pin.o Psq.o protlib.o
	g++ -o projet main.o Algo.o Blosum.o file.o Phr.o Pin.o Psq.o protlib.o
main.o: main.cpp Algo.h Blosum.h file.h Phr.h Pin.h protlib.hh psq.h
	g++ -c main.cpp Algo.cpp
Algo.o: Algo.cpp Algo.h Blosum.h
	g++ -c Algo.cpp
Blosum.o: Blosum.cpp Blosum.h
	g++ -c Blosum.cpp
file.o: file.cpp file.h
	g++ -c file.cpp
Phr.o: Phr.cpp Phr.h file.h
	g++ -c Phr.cpp
Pin.o: Pin.cpp Pin.h file.h
	g++ -c Pin.cpp
Psq.o: Psq.cpp psq.h file.h
	g++ -c Psq.cpp
protlib.o: protlib.cc protlib.hh
	g++ -c protlib.cc
clean :
	rm -f prog *.o
