#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "../src/ui/RoundedRectangle.hpp"
class Screen
{
protected:
    Screen * gScreen; // need to be able to call setup on this
public:
    int index = -1;

    int MainMenuIdx;
    int PauseMenuIdx;
    int GameScreenIdx;
    int ServerPickerIdx;
    int SettingsIdx;

    sf::Font textFont1;
    sf::Font textFont2;
    sf::Font headerFont;
    sf::Font subMenuFont;
    
    bool fontError = false;
    bool text1FontL = true;
    bool text2FontL = true;
    bool headerFontL = true;
    bool subMenuFontL = true;

    sf::Sound menuBeep;
    sf::SoundBuffer menuSoundBuf;
    
    sf::View defaultView;
    void setDefaultView(sf::View v)
    {
        defaultView = v;
    }

    void setGameScreen(Screen * gs)
    {
        gScreen = gs;
    }

    //virtual void openGame(sf::RenderWindow & window, bool local){}
    //virtual void closeGame(){}

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

        if(menuSoundBuf.loadFromFile("res/shortBeep.wav"))
            menuBeep.setBuffer(menuSoundBuf);
    }
    ~Screen()
    {

    }

    bool within(int checkx, int checky, int x1, int x2, int y1, int y2)
    {
        if(checkx >= x1 && checkx <= x2 && checky >= y1 && checky <= y2)
            return true;
        return false;

    }

    void playMenuHiliteSound()
    {
        menuBeep.play();
    }

    virtual int Run(sf::RenderWindow &Program) = 0;

};

#endif
