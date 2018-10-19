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

    sf::View defaultView = window.getView();

    MainMenu mainMenu;
    Settings settings;
    GameScreen gameScreen;
    PauseMenu pauseMenu;
    ServerPicker serverPicker;

    Screens.push_back(&mainMenu);   // 0 - Main Menu 
    Screens.push_back(&gameScreen); // 1 - Game Screen
    Screens.push_back(&settings);   // 2 - Settings Screen
    Screens.push_back(&pauseMenu);  // 3 - Pause Menu
    Screens.push_back(&serverPicker);  // 3 - Pause Menu

    // Give each screen an index
    for(int i = 0; i < Screens.size(); i++)
    {
        Screens[i]->index = i; 
        Screens[i]->setGameScreen((Screen*)&gameScreen);
        Screens[i]->setDefaultView(defaultView);
    }
    // Then set their indices 
    for(int i = 0; i < Screens.size(); i++)
    {
        Screens[i]->MainMenuIdx = mainMenu.index;
        Screens[i]->SettingsIdx = settings.index;
        Screens[i]->PauseMenuIdx = pauseMenu.index;
        Screens[i]->GameScreenIdx = gameScreen.index;
        Screens[i]->ServerPickerIdx = serverPicker.index;
    }
    

    while(screen != -1)
    {
        screen = Screens[screen]->Run(window);
    }
    gameScreen.closeGame();

	return 0;
}

