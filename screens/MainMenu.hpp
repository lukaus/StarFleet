#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include <iostream>
#include <string>
#include <vector>
#include "Screen.hpp"
#include "GameScreen.hpp"

using namespace std;
class MainMenu : public Screen
{
private:
    GameScreen * gameScreen; // need to be able to call setup on this
    int hilite = -1;
    void scaleToWidth(sf::Text& obj, double desiredScale, int width)
    {
        string objString = obj.getString();
        int stringLength = objString.length();
        float scaleFactor = (width / (obj.getGlobalBounds().width+(obj.getCharacterSize()) / stringLength));
        obj.setScale(scaleFactor * desiredScale, scaleFactor * desiredScale );
    }
    void scaleToHeight(sf::Text& obj, double desiredScale, int height)
    {
        string objString = obj.getString();
        int stringLength = objString.length();
        float scaleFactor = (height / (obj.getGlobalBounds().height ));
        obj.scale(scaleFactor * desiredScale, scaleFactor * desiredScale);
    }

public:
    int Run(sf::RenderWindow & window)
    {
        gameScreen = (GameScreen*)gScreen;
        int selection = index;

        vector<sf::Drawable*> uiElements;

        sf::Text title;
        sf::Text playTxt;
        sf::Text optionsTxt;
        sf::Text serverTxt;
        sf::Text quitTxt;

        playTxt.setFont(subMenuFont);
        optionsTxt.setFont(subMenuFont);
        serverTxt.setFont(subMenuFont);
        quitTxt.setFont(subMenuFont);

        playTxt.setString("LOCAL");
        serverTxt.setString("CONNECT");
        optionsTxt.setString("OPTIONS");
        quitTxt.setString("QUIT");

        playTxt.setFillColor(sf::Color(22, 22, 22));
        optionsTxt.setFillColor(sf::Color(22, 22, 22));
        serverTxt.setFillColor(sf::Color(22, 22, 22));
        quitTxt.setFillColor(sf::Color(22, 22, 22));

        playTxt.setCharacterSize(1000);
        optionsTxt.setCharacterSize(1000);
        serverTxt.setCharacterSize(1000);
        quitTxt.setCharacterSize(1000);

        scaleToWidth(playTxt, 0.6, window.getSize().x * 0.6);
        scaleToWidth(optionsTxt, 0.6, window.getSize().x * 0.6);
        scaleToWidth(serverTxt, 0.6, window.getSize().x * 0.6);
        scaleToWidth(quitTxt, 0.6, window.getSize().x * 0.6);

        title.setFont(headerFont);
        title.setString("STAR FLEET");
        title.setFillColor(sf::Color(255, 153, 0));
        //Calculate text positions 
        title.setCharacterSize(1000);
        title.setPosition((window.getSize().x / 5), 0);
        uiElements.push_back(&title);

        // With above text properties, this scale factor will 
        scaleToWidth(title, 0.6, window.getSize().x);

        float buttonWidth = window.getSize().x / 5;
        float buttonHeight = buttonWidth / 5;

        scaleToHeight(playTxt, 0.8, buttonHeight);
        scaleToHeight(optionsTxt, 0.8, buttonHeight);
        scaleToHeight(serverTxt, 0.8, buttonHeight);
        scaleToHeight(quitTxt, 0.8, buttonHeight);

        sf::Vector2f size(buttonWidth, buttonHeight);
        sf::RoundedRectangleShape playBtn(size, 10.0, 10);
        playBtn.setCornersRadius(5);
        playBtn.setFillColor(sf::Color(204, 153, 204));
        uiElements.push_back(&playBtn);
        playBtn.setPosition((window.getSize().x / 5) * 2, (title.getPosition().y + title.getGlobalBounds().height) + 100);

        sf::RoundedRectangleShape playServerBtn(size, 10.0, 10);
        playServerBtn.setCornersRadius(5);
        playServerBtn.setFillColor(sf::Color(204, 153, 204));
        uiElements.push_back(&playServerBtn);
        playServerBtn.setPosition((window.getSize().x / 5) * 2, (playBtn.getPosition().y + playBtn.getGlobalBounds().height) + 10);

        sf::RoundedRectangleShape optionsBtn(size, 10.0, 10);
        optionsBtn.setCornersRadius(5);
        optionsBtn.setFillColor(sf::Color(204, 153, 204));
        uiElements.push_back(&optionsBtn);
        optionsBtn.setPosition((window.getSize().x / 5) * 2, (playServerBtn.getPosition().y + playServerBtn.getGlobalBounds().height) + 10);

        sf::RoundedRectangleShape quitBtn(size, 10.0, 10);
        quitBtn.setCornersRadius(5);
        quitBtn.setFillColor(sf::Color(204, 102, 153));
        uiElements.push_back(&quitBtn);
        quitBtn.setPosition((window.getSize().x / 5) * 2, (optionsBtn.getPosition().y + optionsBtn.getGlobalBounds().height*3) + 10);

        playTxt.setPosition(playBtn.getPosition().x + (buttonWidth / 5),           playBtn.getPosition().y - (buttonHeight / 2.5));
        optionsTxt.setPosition(optionsBtn.getPosition().x + (buttonWidth / 10),     optionsBtn.getPosition().y - (buttonHeight / 2.5));
        serverTxt.setPosition(playServerBtn.getPosition().x + (buttonWidth / 12),   playServerBtn.getPosition().y - (buttonHeight / 2.5));
        quitTxt.setPosition(quitBtn.getPosition().x + (buttonWidth / 3.2),          quitBtn.getPosition().y - (buttonHeight / 4.0));

        uiElements.push_back(&playTxt);
        uiElements.push_back(&optionsTxt);
        uiElements.push_back(&serverTxt);
        uiElements.push_back(&quitTxt);

        sf::Event event;
        while (window.pollEvent(event))
        {
            sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            int oldHilite = hilite;
            switch (event.type)
            {
                case sf::Event::MouseMoved:
                    if(within(mPos.x, mPos.y, playBtn.getPosition().x, playBtn.getPosition().x + buttonWidth, playBtn.getPosition().y, playBtn.getPosition().y + buttonHeight))
                    {
                        hilite = 0;
                    }
                    else if(within(mPos.x, mPos.y, playServerBtn.getPosition().x, playServerBtn.getPosition().x + buttonWidth, playServerBtn.getPosition().y, playServerBtn.getPosition().y + buttonHeight))
                    {
                        hilite = 1;
                    }
                    else if(within(mPos.x, mPos.y, optionsBtn.getPosition().x, optionsBtn.getPosition().x + buttonWidth, optionsBtn.getPosition().y, optionsBtn.getPosition().y + buttonHeight))
                    {
                        hilite = 2;
                    }
                    else if(within(mPos.x, mPos.y, quitBtn.getPosition().x, quitBtn.getPosition().x + buttonWidth, quitBtn.getPosition().y, quitBtn.getPosition().y + buttonHeight))
                    {
                        hilite = 3;
                    }
                    else
                        hilite = -1;
                    
                    break;

                case sf::Event::KeyPressed:
                    break;
                case sf::Event::MouseButtonPressed:
                    switch(hilite)
                    {
                        case 0:
                            gameScreen->openGame(window, true);
                            selection = GameScreenIdx;
                            break;
                        case 1:
                            gameScreen->setServerInfo((char *)"localhost", 8081);
                            gameScreen->openGame(window, false);
                            selection = GameScreenIdx;
                            break;
                        case 2:
                            gameScreen->fromScreen = index;
                            selection = SettingsIdx;
                            break;
                        case 3:
                            selection = -1;
                            break;
                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }
            if(hilite != -1 && oldHilite != hilite)
                playMenuHiliteSound();
            if (event.type == sf::Event::Resized)
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
        }
        
        switch(hilite)
        {
            case 0:
                playBtn.setFillColor(sf::Color(153, 153, 204));
                playTxt.setFillColor(sf::Color(255, 255, 255));
                break;
            case 1:
                playServerBtn.setFillColor(sf::Color(153, 153, 204));
                serverTxt.setFillColor(sf::Color(255, 255, 255));
                break;
            case 2:
                optionsBtn.setFillColor(sf::Color(153, 153, 204));
                optionsTxt.setFillColor(sf::Color(255, 255, 255));
                break;
            case 3:
                quitBtn.setFillColor(sf::Color(153, 153, 204));
                quitTxt.setFillColor(sf::Color(255, 255, 255));
                break;
            default:

                break;
        }

        window.clear();
        for(int i = 0; i < uiElements.size(); i++)
            window.draw(*uiElements[i]);

        window.display();
        return selection; 
    }
};

#endif
