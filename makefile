MAIN		= main.cpp
PROGRAMS	= HexGrid.cpp Crewman.cpp Ship.cpp Protocol.cpp Projectile.cpp
COMPFLAGS	= -std=c++11 -o
LINKFLAGS	= -lsfml-graphics -lsfml-window -lsfml-system -lpthread
COMPILER	= g++
EXECUTABLE	= starFleet

all: main

main : $(MAIN) $(PROGRAMS)
	$(COMPILER) $(COMPFLAGS) $(EXECUTABLE) $(MAIN) $(PROGRAMS) $(LINKFLAGS)

clean:
	rm *.o $(EXECUTABLE)

debug:
	$(COMPILER) $(COMPFLAGS) -ggdb $(EXECUTABLE) $(MAIN) $(PROGRAMS) $(LINKFLAGS)

release:
	g++ -c main.cpp HexGrid.cpp Crewman.cpp Ship.cpp Projectile.cpp Protocol.cpp
	g++ main.o HexGrid.o Crewman.o Ship.o Protocol.o Projectile.o -o starFleet -lsfml-graphics -lsfml-window -lsfml-system -lpthread

assembly:
	g++ -c main.cpp HexGrid.cpp Crewman.cpp Ship.cpp Projectile.cpp Protocol.cpp -S
	g++ main.o HexGrid.o Crewman.o Ship.o Protocol.o Projectile.o -o starFleet -lsfml-graphics -lsfml-window -lsfml-system -lpthread

server: server.cpp Ship.cpp Protocol.cpp
	g++ -c -std=c++11 -ggdb server.cpp Ship.cpp Protocol.cpp
	g++ server.o Ship.o Protocol.o -o server
