#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include <iostream>
#include "Screen.hpp"
#include "GameScreen.hpp"

using namespace std;
class MainMenu : public Screen
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
        int selection = 0;

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::KeyPressed:
                    //selection = 1;
                    break;
                case sf::Event::MouseButtonPressed:
                    selection = 1;
                    break;

                default:
                    break;
            }
            if (event.type == sf::Event::Resized)
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
        }

        window.clear();

        sf::Text title;
        sf::Font menuFont;
        if(!menuFont.loadFromFile("res/Okuda.otf"))
            cerr << "Failed to load menu font: res/big.ttf\n";
        else
            title.setFont(menuFont);
        title.setString("STARFLEET");
        title.setFillColor(sf::Color(255, 153, 0));
        title.setCharacterSize(275);
        title.setPosition(10, -20);

        window.draw(title);

        window.display();
        return selection; 
    }
};



#endif
