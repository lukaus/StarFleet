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
    
    bool fontError = false;
    bool text1FontL = true;
    bool text2FontL = true;
    bool headerFontL = true;
    bool subMenuFontL = true;

    Screen()
    {
        if(!textFont1.loadFromFile("res/textFont.ttf"))
        {
            text1FontL = false;
            std::cerr << "Font: res/textFont.ttf failed to load.\n";
        }
        if(!textFont2.loadFromFile("res/Okuda.otf"))
        {
            text2FontL = false;
            std::cerr << "Font: res/Okuda.otf failed to load.\n";
        }
        if(!headerFont.loadFromFile("res/FINALOLD.TTF"))
        {
            headerFontL = false;
            std::cerr << "Font: res/FINALOLD.TTF failed to load.\n";
        }
        if(!subMenuFont.loadFromFile("res/Jefferies.otf"))
        {
            subMenuFontL = false;
            std::cerr << "Font: res/Jefferies.otf failed to load.\n";
        }
        fontError = (text1FontL && text2FontL && headerFontL && subMenuFontL);

    }

    virtual int Run(sf::RenderWindow &Program) = 0;

};

#endif
