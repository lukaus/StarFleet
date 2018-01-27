

all: 
	g++ -c -g main.cpp HexGrid.cpp Crewman.cpp Ship.cpp 
	g++ main.o HexGrid.o Crewman.o Ship.o -o starfinderCommander -lsfml-graphics -lsfml-window -lsfml-system
	rm main.o HexGrid.o Crewman.o Ship.o

clean:
	rm *.o

debug:

release:
	g++ -c main.cpp HexGrid.cpp Crewman.cpp Ship.cpp 
	g++ main.o HexGrid.o Crewman.o Ship.o -o starfinderCommander -lsfml-graphics -lsfml-window -lsfml-system

assembly:
	g++ -c main.cpp HexGrid.cpp Crewman.cpp Ship.cpp -S
	g++ main.o HexGrid.o Crewman.o Ship.o -o starfinderCommander -lsfml-graphics -lsfml-window -lsfml-system
