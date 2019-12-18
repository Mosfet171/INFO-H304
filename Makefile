# Makefile for proty

proty: protlib.o main.o algo.o blosum.o file.o phr.o psq.o file.o
	g++ proty.o protlib.o -o proty -Wall

protlib.o: protlib.cc protlib.hh
	g++ -c protlib.cc -Wall

main.o: main.cpp protlib.hh
	g++ -c main.cpp -Wall

algo.o: Algo.cpp Algo.h
	g++ -c Algo.cpp -Wall

blosum.o: Blosum.cpp Blosum.h
	g++ -c Blosum.cpp -Wall

file.o: file.cpp file.h
	g++ -c file.cpp -Wall

phr.o: Phr.cpp Phr.h
	g++ -c Phr.cpp -Wall

psq.o: Psq.cpp psq.h
	g++ -c Psq.cpp -Wall

pin.o: Pin.cpp Pin.h
	g++ -c Pin.cpp -Wall

	
