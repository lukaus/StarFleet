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

        for(int i = 0; i < uiElements.size(); i++)
            window.draw(*uiElements[i]);

        window.display();
        return selection; 
    }
};

#endif
