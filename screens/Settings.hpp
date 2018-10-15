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
        int selection = 2;
        window.clear(sf::Color(122, 122, 122));

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::KeyPressed:
                    break;
                case sf::Event::MouseButtonPressed:
                    selection = 0;
                    break;
                default:
                    break;
            }
        }

        window.display();
        return selection; 
    }
};



#endif
