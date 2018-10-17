#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <iostream>
#include "Screen.hpp"
#include "GameScreen.hpp"

using namespace std;
class Settings : public Screen
{
private:
    GameScreen * gameScreen;
public:
    int Run(sf::RenderWindow & window)
    {
        gameScreen = (GameScreen*)gScreen;
        int selection = SettingsIdx;
        window.clear(sf::Color(122, 122, 122));

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::KeyPressed:
                    break;
                case sf::Event::MouseButtonPressed:
                    selection = gameScreen->fromScreen;
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
