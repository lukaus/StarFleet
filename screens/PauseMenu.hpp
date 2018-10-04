#ifndef PAUSEMENU_HPP
#define PAUSEMENU_HPP

#include "Screen.hpp"
#include "GameScreen.hpp"

class PauseMenu : public Screen
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
        
        return 0;
    }

};



#endif
