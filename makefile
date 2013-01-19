CPP=g++
CPPFLAGS=-O3

all: DiamondDashBot

DiamondDashBot: main.cpp util.o
	$(CPP) main.cpp util.o -lgdi32 -o DiamondDashBot $(CPPFLAGS)

util.o: util.cpp util.h
	$(CPP) -c util.cpp $(CPPFLAGS)
