#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <list>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "Screens.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    int screen = 0;
    vector<Screen*> Screens;

    char *serverIp;
    int port;
    // Gets the IP and Port
    if(argc >= 3){
        serverIp = argv[1]; 
        port = atoi(argv[2]);
    }
    // window logic
    sf::RenderWindow window(sf::VideoMode(1270, 720), "Starfinder Commander");
    window.setFramerateLimit(60);

    MainMenu mainMenu;
    Settings settings;
    GameScreen gameScreen;
    PauseMenu pauseMenu;

    gameScreen.setServerInfo(serverIp, port);
    mainMenu.setGameScreen(&gameScreen);
    settings.setGameScreen(&gameScreen);
    pauseMenu.setGameScreen(&gameScreen);

    Screens.push_back(&mainMenu);
    Screens.push_back(&gameScreen);
    Screens.push_back(&settings);
    Screens.push_back(&pauseMenu);

    while(screen != -1)
    {
        screen = Screens[screen]->Run(window);
    }
    gameScreen.closeGame();

	return 0;
}

