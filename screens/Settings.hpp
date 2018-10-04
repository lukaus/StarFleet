#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <iostream>
#include "Screen.hpp"
#include "GameScreen.hpp"

using namespace std;
class Settings : public Screen
{
private:
    GameScreen * gameScreen; // need to be able to call setup on this
public:
    void setGameScreen(GameScreen * gs)
    {
        gameScreen = gs;
    }
    int Run(sf::RenderWindow & window)
    {
        bool play = false;
        window.clear();

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::KeyPressed:
                    play = true;
                    break;
                case sf::Event::MouseButtonPressed:
                    play = true;
                    break;
                default:
                    break;
            }
        }

        if(play)
        {
            return 1;
        }
        window.display();
        return 2; 
    }
};



#endif
