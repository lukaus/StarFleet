#ifndef SERVERPICKER_HPP
#define SERVERPICKER_HPP

#include <iostream>
#include <string>
#include <vector>
#include "Screen.hpp"
#include "GameScreen.hpp"

using namespace std;
class ServerPicker : public Screen
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
        gameScreen = (GameScreen*)gScreen; // need to be able to call setup on this
        int selection = 5;

        vector<sf::Drawable*> uiElements;

        sf::Text title;
        sf::Text cancelTxt;
        sf::Text connectTxt;

        cancelTxt.setFont(subMenuFont);
        connectTxt.setFont(subMenuFont);

        cancelTxt.setString("LOCAL");
        connectTxt.setString("OPTIONS");

        cancelTxt.setFillColor(sf::Color(22, 22, 22));
        connectTxt.setFillColor(sf::Color(22, 22, 22));

        cancelTxt.setCharacterSize(1000);
        connectTxt.setCharacterSize(1000);

        scaleToWidth(cancelTxt, 0.6, window.getSize().x * 0.6);
        scaleToWidth(connectTxt, 0.6, window.getSize().x * 0.6);

        title.setFont(headerFont);
        title.setString("SERVER INFO");
        title.setFillColor(sf::Color(255, 153, 0));
        //Calculate text positions 
        title.setCharacterSize(1000);
        title.setPosition((window.getSize().x / 5), 0);
        uiElements.push_back(&title);

        // With above text properties, this scale factor will 
        scaleToWidth(title, 0.6, window.getSize().x);

        float buttonWidth = window.getSize().x / 5;
        float buttonHeight = buttonWidth / 5;

        scaleToHeight(cancelTxt, 0.8, buttonHeight);
        scaleToHeight(connectTxt, 0.8, buttonHeight);

        sf::Vector2f size(buttonWidth, buttonHeight);
        sf::RoundedRectangleShape cancelBtn(size, 10.0, 10);
        cancelBtn.setCornersRadius(5);
        cancelBtn.setFillColor(sf::Color(204, 153, 204));
        uiElements.push_back(&cancelBtn);
        cancelBtn.setPosition((window.getSize().x / 5) * 2, (title.getPosition().y + title.getGlobalBounds().height) + 100);

        sf::RoundedRectangleShape connectButton(size, 10.0, 10);
        connectButton.setCornersRadius(5);
        connectButton.setFillColor(sf::Color(204, 153, 204));
        uiElements.push_back(&connectButton);
        connectButton.setPosition((window.getSize().x / 5) * 2, (cancelBtn.getPosition().y + cancelBtn.getGlobalBounds().height) + 10);

        cancelTxt.setPosition(cancelBtn.getPosition().x + (buttonWidth / 5),           cancelBtn.getPosition().y - (buttonHeight / 2.5));
        connectTxt.setPosition(connectButton.getPosition().x + (buttonWidth / 12),   connectButton.getPosition().y - (buttonHeight / 2.5));

        uiElements.push_back(&cancelTxt);
        uiElements.push_back(&connectTxt);

        sf::Event event;
        while (window.pollEvent(event))
        {
            sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            int oldHilite = hilite;
            switch (event.type)
            {
                case sf::Event::MouseMoved:
                    if(within(mPos.x, mPos.y, cancelBtn.getPosition().x, cancelBtn.getPosition().x + buttonWidth, cancelBtn.getPosition().y, cancelBtn.getPosition().y + buttonHeight))
                    {
                        hilite = 0;
                    }
                    else if(within(mPos.x, mPos.y, connectButton.getPosition().x, connectButton.getPosition().x + buttonWidth, connectButton.getPosition().y, connectButton.getPosition().y + buttonHeight))
                    {
                        hilite = 1;
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
                            selection = 1;
                            break;
                        case 1:
                            gameScreen->setServerInfo((char *)"localhost", 8081);
                            gameScreen->openGame(window, false);
                            selection = 1;
                            break;
                        case 2:
                            selection = 2;
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
                cancelBtn.setFillColor(sf::Color(153, 153, 204));
                cancelTxt.setFillColor(sf::Color(255, 255, 255));
                break;
            case 1:
                connectButton.setFillColor(sf::Color(153, 153, 204));
                connectTxt.setFillColor(sf::Color(255, 255, 255));
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
