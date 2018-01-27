

all: 
	g++ -c -std=c++11 -g main.cpp HexGrid.cpp Crewman.cpp Ship.cpp 
	g++ main.o HexGrid.o Crewman.o Ship.o -o starFleet -lsfml-graphics -lsfml-window -lsfml-system -lpthread
	rm main.o HexGrid.o Crewman.o Ship.o

clean:
	rm *.o

debug:

release:
	g++ -c main.cpp HexGrid.cpp Crewman.cpp Ship.cpp 
	g++ main.o HexGrid.o Crewman.o Ship.o -o starFleet -lsfml-graphics -lsfml-window -lsfml-system -lpthread

assembly:
	g++ -c main.cpp HexGrid.cpp Crewman.cpp Ship.cpp -S
	g++ main.o HexGrid.o Crewman.o Ship.o -o starFleet -lsfml-graphics -lsfml-window -lsfml-system -lpthread
