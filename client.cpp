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

    if(argc != 3)
    {
        cerr << "Usage: ip_address port" << endl; exit(0); 
    } 
    // Gets the IP and Port
    char *serverIp = argv[1]; 
    int port = atoi(argv[2]);

    // window logic
    sf::RenderWindow window(sf::VideoMode(1270, 720), "Starfinder Commander");
    window.setFramerateLimit(60);

    GameScreen gameScreen;
    gameScreen.setServerInfo(serverIp, port);

    Screens.push_back(&gameScreen);

    while(screen != -1)
    {
        screen = Screens[screen]->Run(window);
    }

	return 0;
}

