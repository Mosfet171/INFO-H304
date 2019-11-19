# Makefile for proty

proty: protlib.o proty.o
	g++ proty.o protlib.o -o proty -Wall

protlib.o: protlib.cc protlib.hh
	g++ -c protlib.cc -Wall

proty.o: proty.cpp protlib.hh
	g++ -c proty.cpp -Wall
	