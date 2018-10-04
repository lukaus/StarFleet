#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <SFML/Graphics.hpp>
class Screen
{
public:
    virtual int Run(sf::RenderWindow &Program) = 0;
};

#endif
