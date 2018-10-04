#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <list>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <thread>
#include "../src/HexGrid.h"
#include "../src/Ship.h"
#include "../src/Crewman.h"
#include "../src/Projectile.h"
#include "../src/Protocol.h"
#include "Screen.hpp"

#define DRAG_TIMEOUT 200			// in milliseconds
#define DOUBLE_CLICK_TIMEOUT 500	// in milliseconds
#define INPUT_DELAY 100 // milliseconds

using namespace std;

class GameScreen : public Screen
{
private:
    // Global variable for the client socket descripter...needed for thread
    char* serverIp;
    int port;
public:
    enum class MsgType : char{
        ClientID = 'C',
        Ships = 'S',
        Projectiles = 'P',
        Invalid = 'Z'
    };

    class DrawShip
    {
    private:
        Ship* ship;
        sf::Sprite* sprite;
        sf::Texture* tex;

        bool myTurn = true;
    public:

        DrawShip()
        {
            this->sprite = new sf::Sprite();
            this->tex = new sf::Texture();
        }

        DrawShip(const DrawShip& cpy)
        {
            delete this->sprite;
            delete this->tex;

            this->ship = new Ship(*cpy.getShip());
            this->sprite = new sf::Sprite(*cpy.getSprite());
            this->tex = new sf::Texture(*cpy.getTex());
        }

            ~DrawShip()
        {
            delete tex;
            this->tex = nullptr;

            delete sprite;
            this->sprite = nullptr;
        }
        
        DrawShip& operator=(DrawShip rhs)
        {
            delete this->ship;
            delete this->sprite;
            delete this->tex;

            this->ship = new Ship(*rhs.getShip());
            this->sprite = new sf::Sprite(*rhs.getSprite());
            this->tex = new sf::Texture(*rhs.getTex());

            return *this;   
        }

        void setTex(sf::Texture* t)
        {
            tex = t;
        }

        sf::Texture* getTex() const
        {
            return tex;
        }

        void setShip(Ship* sh)
        {
            ship = sh;
        }

        Ship * getShip() const
        {
            return this->ship;
        }

        int getXpos()
        {
            return this->ship->getXpos();
        }
        
        int getYpos()
        {
            return this->ship->getYpos();
        }

        sf::Vector2f position()
        {
            return sf::Vector2f((float)this->ship->getXpos(), (float)this->ship->getYpos());
        }
        
        void setSprite(sf::Sprite* sp)
        {
            sprite = sp;
        }
        
        sf::Sprite* getSprite() const
        {
            return this->sprite;
        }

        void Draw(sf::RenderWindow &window, HexGrid &grid)
        {
            this->sprite->setPosition(grid.offset_to_pixel(sf::Vector2f((float)this->ship->getXpos(), (float)this->ship->getYpos())  ));
            this->sprite->setRotation(60.0 * this->ship->getOrientation());

            window.draw( *this->sprite );
        }

        void Move(HexGrid grid, int x, int y)
        {
            if(ValidCoordinates(grid, x, y) == false || myTurn == false)
            {
                return;
            }

            this->ship->setXpos(x);
            this->ship->setYpos(y);
        }

        void Orient(HexGrid grid, int dir)
        {
            this->ship->setOrientation((Orientation) dir);
        }

        bool ValidCoordinates(HexGrid grid, int x, int y)
        {
            if(x < 0 || x > grid.getCols() || y < 0 || y > grid.getRows())
                return false;
            return true;
        }

        void Forward(HexGrid grid)
        {
            if(myTurn == false)
                return;
            int gridCols = grid.getCols() - 1;
            int gridRows = grid.getRows() - 1;
            switch (this->ship->getOrientation())
            {
            case EAST:
                if (this->ship->getXpos() == gridCols)
                    return;
                this->ship->setXpos(this->ship->getXpos() + 1);
                this->ship->setYpos(this->ship->getYpos() + 0);
                break;

            case NORTHEAST:
                if ((this->ship->getXpos() == gridCols && this->ship->getYpos() % 2 == 0) || this->ship->getYpos() == 0)
                    return;
                if(this->ship->getYpos() % 2 == 0 )
                    this->ship->setXpos(this->ship->getXpos() + 1);
                this->ship->setYpos(this->ship->getYpos() - 1);
                break;

            case NORTHWEST:
                if ((this->ship->getXpos() == 0 && this->ship->getYpos() % 2 != 0) || this->ship->getYpos() == 0)
                    return;
                if (this->ship->getYpos() % 2 != 0)
                    this->ship->setXpos(this->ship->getXpos() - 1);
                this->ship->setYpos(this->ship->getYpos() - 1);
                break;

            case WEST:
                if (this->ship->getXpos() == 0)
                    return;
                this->ship->setXpos(this->ship->getXpos() - 1);
                this->ship->setYpos(this->ship->getYpos() + 0);
                break;

            case SOUTHWEST:
                if ((this->ship->getXpos() == 0 && this->ship->getYpos() % 2 != 0) || this->ship->getYpos() == gridRows)
                    return;
                if (this->ship->getYpos() % 2 != 0)
                    this->ship->setXpos(this->ship->getXpos() - 1);
                this->ship->setYpos(this->ship->getYpos() + 1);
                break;

            case SOUTHEAST:

                if ((this->ship->getXpos() == gridCols && this->ship->getYpos() % 2 == 0) || this->ship->getYpos() == gridRows)
                    return;
                if (this->ship->getYpos() % 2 == 0)
                    this->ship->setXpos(this->ship->getXpos() + 1);
                this->ship->setYpos(this->ship->getYpos() + 1);
                break;
            }
        }
        void Back(HexGrid grid)
        {

        }
        void Right()
        {
            if (this->ship->getOrientation() == 5)
                this->ship->setOrientation((Orientation)0);
            else
                this->ship->setOrientation((Orientation) ( (int) this->ship->getOrientation() + 1) );
        }
        void Left()
        {
            if (this->ship->getOrientation() == 0)
                this->ship->setOrientation((Orientation)5);
            else
                this->ship->setOrientation((Orientation)((int)this->ship->getOrientation() - 1));
        }
    };

    //void DrawShips(sf::RenderWindow & window, HexGrid &grid, vector<DrawShip*> & shipList);
    //DrawShip * GetShipHere(sf::Vector2f pos, vector<DrawShip*> & shipList, int& selShpInd);
    //void CheckDrawShips(vector<DrawShip*>& drawShips, vector<Ship*>& ships, int& cid);

    // Thread to check for server sending messages
    static void checkerThread(vector<Ship*>* ships, int* cid, int* clientSd)
    {
        // Buffer for the message incoming
        char receivedMessage[1500];
        while (1)
        {
            // Clear buffer
            memset(receivedMessage, 0, sizeof(receivedMessage));

            // Try to receive a message from the server
            recv(*clientSd, (char*)&receivedMessage, sizeof(receivedMessage), 0);

            if(!strcmp(receivedMessage, "exit"))
            {
                cerr << "Server has quit the session" << endl;
                exit(0);
            }

            int size = 0;
            for(int i = 0; receivedMessage[i] != '\0'; i++)
                size += 1;

            MsgType msgType = MsgType::Invalid;
            memcpy(&msgType, &receivedMessage[0], sizeof(char));
            std::vector<Ship*> resultShips;
            int fromServer = -1;
            switch(static_cast<char>(msgType))
            {
                case static_cast<char>(MsgType::ClientID):
                    *cid = Protocol::ParseClientIDMessage(receivedMessage, size);
                    cerr << "Client ID received : " << *cid << "\n";
                    while(ships->size() < *cid)
                    {
                        cerr << "Ship size is " << ships->size() << "Adding entry\n"; 
                        ships->push_back(new Ship());
                    }
                    break;

                case static_cast<char>(MsgType::Ships):

                    resultShips = Protocol::ParseShipMessage(*clientSd, receivedMessage, size, fromServer);
                    for(int i = ships->size()-1; i >=0; i--)
                    {
                        delete ships->at(i);
                        ships->at(i) = nullptr;
                        ships->erase(ships->begin() + i);
                    }

                    *ships = resultShips;
                    break;

                case static_cast<char>(MsgType::Projectiles):
                    break;

                default:
                    break;
            }
        }
    }

    void setServerInfo(char* sip, int p)
    {
        serverIp = sip;
        port = p;
    }

    int Run(sf::RenderWindow & window)
    {
        // Setup a socket and connection tools 
        struct hostent* host = gethostbyname(serverIp); 
        sockaddr_in sendSockAddr;   
        bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 
        sendSockAddr.sin_family = AF_INET; 
        sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
        sendSockAddr.sin_port = htons(port);
        int clientSd = socket(AF_INET, SOCK_STREAM, 0);

        // Try to connect
        int status = connect(clientSd, (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));

        if(status < 0)
        {
            cerr << "Error connecting to server!" << endl;
            exit(0);
        }
        else
            cout << "Connected to the server!" << endl;

        std::vector<Ship*> ships;
        int cid = 0; // index for this client's DrawShip in ships vector

        // Spawn the thread to check for incoming messages from the server
        thread t1(checkerThread, &ships, &cid, &clientSd);
        // window logic
        window.setFramerateLimit(60);
        sf::Vector2f origin;
        origin.x = 100;
        origin.y = 100;
        HexGrid grid(0, 0, 100, 100, 20, sf::LinesStrip);

        sf::VertexArray hexGrid;
        hexGrid.setPrimitiveType(grid.GetPrimitiveType());
        hexGrid = grid.GenerateHexGrid();

        bool leftMouseDragging = false;
        bool potentialDoubleLeftClick = false;
        sf::View camera = window.getView();
        sf::View hud = sf::View(window.getView());

        sf::Vector2u winSize = window.getSize();
        sf::Vector2f mPos_old = window.getView().getCenter();
        
        bool shipSelected;
        int selectedShipIndex = -1;
        Ship * selectedShip = NULL;

        sf::Font font;
        if (!font.loadFromFile("res/big.ttf")) {
            cerr << "Failed locate font: \"res/testFont.ttf\" in executable directory\n";
            return -1;
        }
        sf::Text hudText;
        hudText.setFont(font);
        hudText.setPosition(0,-15);
        string mystring = ("~MYSHIP~\nSHIELD:\n\tF:\n\tB:\n\tL:\n\tR:\nHEALTH:\nSPEED:\nPOWER:\n\tCURR:\n\tAVAIL:\n\t");
        hudText.setString(mystring);
        hudText.setCharacterSize(35);
        hudText.setFillColor(sf::Color(255,255,255,255));
        hudText.setStyle(sf::Text::Bold);
        sf::Clock leftDragTimer;
        sf::Clock doubleLeftClickTimer;

        // Create initial client ship
        Ship* shp = new Ship();
        shp->setXpos(cid);
        shp->setYpos(cid);
        shp->setOwner(cid);
        shp->setID(cid);
        shp->setArmourClass(20);
        shp->setTargetLock(10);
        shp->setHullPointsMax(100);
        shp->setHullPointsCur(53);
        shp->setShieldMax(Shield::Fore, 77);
        shp->setShieldMax(Shield::Aft, 78);
        shp->setShieldMax(Shield::Port, 79);
        shp->setShieldMax(Shield::Starboard, 90);
        shp->setShieldCur(Shield::Fore, 91);
        shp->setShieldCur(Shield::Aft, 92);
        shp->setShieldCur(Shield::Port, 93);
        shp->setShieldCur(Shield::Starboard, 95);
        ships.push_back(shp);
        // Game logic
        vector<DrawShip*> drawShips;
        sf::Clock inputDelayTimer;

#pragma region GameLogic
        // Interface items
        sf::CircleShape selector(20, 6);
        sf::Vector2f selectorPosition = selector.getOrigin();

        selectorPosition.x = selector.getOrigin().x + (selector.getLocalBounds().height / 2);
        selectorPosition.y = selector.getOrigin().y + (selector.getLocalBounds().height / 2);

        selector.setOrigin(selectorPosition);
        selector.setFillColor(sf::Color(0, 0, 255, 100));

        selector.setPosition(sf::Vector2f(grid.offset_to_pixel(sf::Vector2f(999, 999))));

        sf::CircleShape selectedShipOverlay(20,6);
        sf::Vector2f selectedShipPosition = selectedShipOverlay.getOrigin();

        selectedShipPosition.x = selectedShipOverlay.getOrigin().x + (selectedShipOverlay.getLocalBounds().height / 2);
        selectedShipPosition.y = selectedShipOverlay.getOrigin().y + (selectedShipOverlay.getLocalBounds().height / 2);

        selectedShipOverlay.setOrigin(selectedShipPosition);
        selectedShipOverlay.setFillColor(sf::Color(255, 255, 0, 50));

        selectedShipOverlay.setPosition(sf::Vector2f(grid.offset_to_pixel(sf::Vector2f(999,999))));

#pragma endregion
        float delt;
        int clickCount = 0;
        bool rotated = false;
        sf::Vector2f mousePos;

        winSize = window.getSize();
        sf::Event event;
        inputDelayTimer.restart();

        int initial_message_length;
        char * initial_message = Protocol::CrunchetizeMeCapn(cid, ships, initial_message_length); 
        send(clientSd, initial_message, initial_message_length, 0);

        delete initial_message;
        initial_message = nullptr;
       
        while (window.isOpen())
        {
            CheckDrawShips(drawShips, ships, cid);   // Make sure ship list is still up to date (might make more sense for CheckerThread to just do this? 
                                // hopefully there won't be any race condition type things that happen)
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
                switch (event.type)
                {
#pragma region movement

                case sf::Event::MouseButtonPressed:
                    mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        sf::Vector2f gridOrigin = grid.getOrigin();

                        // left button down
                        leftMouseDragging = true;
                        mPos_old = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        leftDragTimer.restart();
                    }
                    if (event.mouseButton.button == sf::Mouse::Right)
                    {
                        // right button down, set highlight selector
                        sf::Vector2f clickPosition = grid.pixel_to_offset(sf::Vector2f(mousePos.x, mousePos.y));
                        selector.setPosition(grid.offset_to_pixel(clickPosition));
                        // check that grid for a ship at the new position
                        int oldSelectIndex = selectedShipIndex;
                        DrawShip* shipHere = GetShipHere(clickPosition, drawShips, selectedShipIndex);
                        if(selectedShipIndex == -1)
                        {
                            shipSelected = false;
                            selectedShip = NULL;
                            selectedShipIndex = -1;
                            selectedShipOverlay.setPosition(grid.offset_to_pixel(sf::Vector2f(999,999)));
                        }
                        else
                        {
                            oldSelectIndex = selectedShipIndex;
                            selectedShipOverlay.setPosition(grid.offset_to_pixel(clickPosition));
                            selector.setPosition(sf::Vector2f(grid.offset_to_pixel(sf::Vector2f(999,999))));
                            shipSelected = true;
                            selectedShip = shipHere->getShip();
                        }
                    }
                    if (event.mouseButton.button == sf::Mouse::Middle)
                    {
                        // middle button down
                    }
                    break;

                case sf::Event::MouseButtonReleased:
                    mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        // left button up
                        leftMouseDragging = false;
                        if (leftDragTimer.getElapsedTime().asMilliseconds() < DRAG_TIMEOUT)
                        {
                            // Click or double click
                            clickCount++;

                            if (clickCount == 1)
                            {
                                doubleLeftClickTimer.restart();
                            }

                            if (clickCount > 1 && doubleLeftClickTimer.getElapsedTime().asMilliseconds() < DOUBLE_CLICK_TIMEOUT)
                            {
                                clickCount = 0;
                                doubleLeftClickTimer.restart();
                            }
                            else if (clickCount > 2 || doubleLeftClickTimer.getElapsedTime().asMilliseconds() > DOUBLE_CLICK_TIMEOUT)
                            {
                                clickCount = 0;
                                doubleLeftClickTimer.restart();

                            }
                        }
                    }

                    if (event.mouseButton.button == sf::Mouse::Right)
                    {
                        // right button up

                    }
                    if (event.mouseButton.button == sf::Mouse::Middle)
                    {
                        // middle button up

                    }
                    break;

                case sf::Event::MouseMoved:
                    if (leftMouseDragging)
                    {
                        // move window

                        sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                        float xMove = mPos_old.x - mPos.x;
                        float yMove = mPos_old.y - mPos.y;


                        camera.move(xMove, yMove);

                        // limit camera to bounds 0,0 - 3405, 2967 
                        if (camera.getCenter().x < 0)
                            camera.setCenter(0.0, camera.getCenter().y);
                        if (camera.getCenter().x > 3406)
                            camera.setCenter(3406, camera.getCenter().y);
                        if (camera.getCenter().y < 0)
                            camera.setCenter(camera.getCenter().x, 0.0);
                        if (camera.getCenter().y > 2968)
                            camera.setCenter(camera.getCenter().x, 2968);

                        window.setView(camera);
                    }
                    mPos_old = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    break;
                case sf::Event::MouseWheelScrolled:
                    delt = event.mouseWheelScroll.delta;
                    if (delt == 1.0)
                    {
                        camera.zoom(0.8);
                        if (camera.getSize().x < 80)
                        {
                            camera.zoom(1.25);
                        }
                        window.setView(camera);
                    }
                    if (delt == -1.0)
                    {
                        camera.zoom(1.25);
                        if (camera.getSize().x > 5000)
                        {
                            camera.zoom(0.8);
                        }

                        window.setView(camera);
                    }

                    break;
                case sf::Event::KeyPressed:
                    if(event.key.code == sf::Keyboard::R)
                    {
                        if (rotated)
                        {
                            camera.rotate(-30.0);
                        }
                        else
                        {
                            camera.rotate(30.0);
                        }
                        window.setView(camera);
                        rotated = !rotated;
                    }
                    break;
                }
                if (event.type == sf::Event::Resized)
                {
                    // update the view to the new size of the window
                    sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                    camera = sf::View(visibleArea);
                    hud = sf::View(visibleArea);
                  
                    window.setView(sf::View(visibleArea));
                }
#pragma endregion
#pragma region Selection

#pragma endregion
                if(inputDelayTimer.getElapsedTime().asMilliseconds() < INPUT_DELAY)
                    continue;
                inputDelayTimer.restart();
            
#pragma region testMovement
                bool moved = false;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                {
                    moved = true;
                    drawShips[cid]->Left();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                {
                    moved = true;
                    drawShips[cid]->Right();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                {
                    moved = true;
                    drawShips[cid]->Forward(grid);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                {
                    moved = true;
                    drawShips[cid]->Back(grid);
                }

                if(moved)
                {
                    int message_length;
                    char * message = Protocol::CrunchetizeMeCapn(cid, ships, message_length); 
                    send(clientSd, message, message_length, 0);

                    delete message;
                    message = nullptr;
                }
                
                if(shipSelected && selectedShipIndex != -1)
                {
                    selectedShipOverlay.setPosition(sf::Vector2f(grid.offset_to_pixel( drawShips[selectedShipIndex]->position() )));     
                }
            }

#pragma endregion

            window.clear();
            
            window.setView(camera);
            window.draw(hexGrid);
            DrawShips(window, grid, drawShips);
            window.draw(selector);
            window.draw(selectedShipOverlay);

            window.setView(hud);
            window.draw(hudText);
            window.display();
            window.setView(camera);
        }

        t1.join();
        close(clientSd);
        return 0;
    }

    void closeGame()
    {
        //t1.join();
        //close(cliestSd);
    }

    void DrawShips(sf::RenderWindow &window, HexGrid &grid, vector<DrawShip*> & shipList)
    {
        for (int i = 0; i < shipList.size(); i++)
        {
            shipList[i]->Draw(window, grid);
        }
    }

    DrawShip * GetShipHere(sf::Vector2f pos, vector<DrawShip*> & shipList, int& selShpInd)
    {
        for(int i = 0; i < shipList.size(); i++)
        {
            if(shipList[i]->getXpos() == pos.x && shipList[i]->getYpos() == pos.y)
            {
                selShpInd = i;
                return shipList[i];
            }
        }
        selShpInd = -1;
        return NULL;
    }

    // Properly populate the drawShip array based on the cid and the number of ships from server
    void CheckDrawShips(vector<DrawShip*>& drawShips, vector<Ship*>& ships, int& cid)
    {
        for (int i = drawShips.size()-1; i >= 0; i--)
        {
            delete drawShips[i];
            drawShips[i] = nullptr;
            drawShips.erase(drawShips.begin() + i);
        }
        // Find client ship
        for(int i = 0; i < ships.size(); i++)
        {
            Ship* shp = ships[i];
            string spriteFilename = "";
            if(i == cid)
                spriteFilename = "./images/Sprite1ENG_ON.png";
            else
                spriteFilename = "./images/Sprite2ENG_ON.png";

            DrawShip* drawShp = new DrawShip();
            sf::Sprite* sprt = new sf::Sprite();
            sf::Texture* tex = new sf::Texture();
            if (tex->loadFromFile(spriteFilename))
            {
                sprt->setTexture(*tex);
                sprt->setScale(0.4, 0.4);
                sf::Vector2f origin = sprt->getOrigin();
                origin.x = sprt->getOrigin().x + (sprt->getLocalBounds().width / 2);
                origin.y = sprt->getOrigin().y + (sprt->getLocalBounds().height / 2);
                sprt->setOrigin(origin);
                origin = sf::Vector2f(sprt->getLocalBounds().height, sprt->getLocalBounds().width);

                drawShp->setSprite(sprt);
                drawShp->setShip(shp);
                drawShp->setTex(tex);
                drawShips.push_back(drawShp);
            }
        }
    }
};
#endif
