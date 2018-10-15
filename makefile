MAIN		= client.cpp
SERVER		= server.cpp
PROGRAMS	= Screens.hpp src/HexGrid.cpp src/Crewman.cpp src/Ship.cpp src/Protocol.cpp src/Projectile.cpp
COMPFLAGS	= -std=c++11 -o
LINKFLAGS	= -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lpthread
COMPILER	= g++
EXECUTABLE	= starFleet

all: main

main : $(MAIN) $(PROGRAMS)
	$(COMPILER) $(COMPFLAGS) $(EXECUTABLE) $(MAIN) $(PROGRAMS) $(LINKFLAGS)

clean:
	-@rm *.o $(EXECUTABLE) server vgcore.* *.gch screens/*.gch 2>/dev/null || true

debug:
	$(COMPILER) $(COMPFLAGS) -ggdb $(EXECUTABLE) $(MAIN) $(PROGRAMS) $(LINKFLAGS)

release:
	g++ -c main.cpp HexGrid.cpp Crewman.cpp Ship.cpp Projectile.cpp Protocol.cpp
	g++ main.o HexGrid.o Crewman.o Ship.o Protocol.o Projectile.o -o starFleet -lsfml-graphics -lsfml-window -lsfml-system -lpthread

assembly:
	g++ -c main.cpp HexGrid.cpp Crewman.cpp Ship.cpp Projectile.cpp Protocol.cpp -S
	g++ main.o HexGrid.o Crewman.o Ship.o Protocol.o Projectile.o -o starFleet -lsfml-graphics -lsfml-window -lsfml-system -lpthread

server: $(SERVER) $(PROGRAMS) 
	g++ -c -std=c++11 -ggdb $(SERVER) $(PROGRAMS) 
	g++ server.o Ship.o Protocol.o -o server
	-@rm *.o *.gch screens/*.gch 2>/dev/null || true
