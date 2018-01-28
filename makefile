

all: 
	g++ -c -std=c++11 -g main.cpp HexGrid.cpp Crewman.cpp Ship.cpp Projectile.cpp Protocol.cpp
	g++ main.o HexGrid.o Crewman.o Ship.o Protocol.o Projectile.o -o starFleet -lsfml-graphics -lsfml-window -lsfml-system -lpthread
	rm main.o HexGrid.o Crewman.o Ship.o Protocol.o Projectile.o

clean:
	rm *.o

debug:

release:
	g++ -c main.cpp HexGrid.cpp Crewman.cpp Ship.cpp Projectile.cpp Protocol.cpp
	g++ main.o HexGrid.o Crewman.o Ship.o Protocol.o Projectile.o -o starFleet -lsfml-graphics -lsfml-window -lsfml-system -lpthread

assembly:
	g++ -c main.cpp HexGrid.cpp Crewman.cpp Ship.cpp Projectile.cpp Protocol.cpp -S
	g++ main.o HexGrid.o Crewman.o Ship.o Protocol.o Projectile.o -o starFleet -lsfml-graphics -lsfml-window -lsfml-system -lpthread

server:
	g++ -c -std=c++11 -g server.cpp Ship.cpp Protocol.cpp
	g++ server.o Ship.o Protocol.o -o server
