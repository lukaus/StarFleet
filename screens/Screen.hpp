#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "../src/ui/RoundedRectangle.hpp"
class Screen
{
public:
    sf::Font textFont1;
    sf::Font textFont2;
    sf::Font headerFont;
    sf::Font subMenuFont;
    Screen()
    {
        if(!textFont1.loadFromFile("res/textFont.ttf"))
            std::cerr << "Font: res/textFont.ttf failed to load.\n";
        if(!textFont2.loadFromFile("res/Okuda.otf"))
            std::cerr << "Font: res/Okuda.otf failed to load.\n";
        if(!headerFont.loadFromFile("res/FINALOLD.TTF"))
            std::cerr << "Font: res/FINALOLD.TTF failed to load.\n";
        if(!subMenuFont.loadFromFile("res/Jefferies.otf"))
            std::cerr << "Font: res/Jefferies.otf failed to load.\n";
    }

    virtual int Run(sf::RenderWindow &Program) = 0;

};

#endif
