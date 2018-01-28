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
#include "HexGrid.h"
#include "Ship.h"
#include "Crewman.h"
#include "Projectile.h"
#include "Protocol.h"
//#include <boost/serialization/list.hpp>

#define DRAG_TIMEOUT 200			// in milliseconds
#define DOUBLE_CLICK_TIMEOUT 500	// in milliseconds
#define INPUT_DELAY 100 // milliseconds
#define SHIP_INTS (15)


using namespace std;

// Global variable for the client socket descripter...needed for thread
int clientSd;

class DrawShip
{
    //friend class boost::serialization::access;
private:
    Ship* ship;
    sf::Sprite* sprite;

    bool myTurn = true;
public:
    void setShip(Ship* sh)
    {
        ship = sh;
    }

    Ship * getShip()
    {
        return ship;
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

    void Draw(sf::RenderWindow &window, HexGrid &grid)
    {
        // this->sprite->setPosition(grid.offset_to_pixel(grid.cube_to_offset(grid.offset_to_cube(sf::Vector2f((float)this->ship->getXpos(), (float)this->ship->getYpos())))));

        this->sprite->setPosition(grid.offset_to_pixel(sf::Vector2f((float)this->ship->getXpos(), (float)this->ship->getYpos())  ));
        this->sprite->setRotation(60.0 * this->ship->getOrientation());

        window.draw( *this->sprite );
    }

    void Move(HexGrid grid, int x, int y)
    {
        if(ValidCoordinates(grid, x, y) == false || myTurn == false)
        {
            cout << "Invalid move, reverted.\n";   
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
            cout << " EAST\n";
            if (this->ship->getXpos() == gridCols)
                return;
            this->ship->setXpos(this->ship->getXpos() + 1);
            this->ship->setYpos(this->ship->getYpos() + 0);
            break;

        case NORTHEAST:
            cout << " NORTHEAST\n";
            if ((this->ship->getXpos() == gridCols && this->ship->getYpos() % 2 == 0) || this->ship->getYpos() == 0)
                return;
            if(this->ship->getYpos() % 2 == 0 )
                this->ship->setXpos(this->ship->getXpos() + 1);
            this->ship->setYpos(this->ship->getYpos() - 1);
            break;

        case NORTHWEST:
            cout << " NORTHWEST\n";

            if ((this->ship->getXpos() == 0 && this->ship->getYpos() % 2 != 0) || this->ship->getYpos() == 0)
                return;
            if (this->ship->getYpos() % 2 != 0)
                this->ship->setXpos(this->ship->getXpos() - 1);
            this->ship->setYpos(this->ship->getYpos() - 1);
            break;

        case WEST:
            cout << " WEST\n";

            if (this->ship->getXpos() == 0)
                return;
            this->ship->setXpos(this->ship->getXpos() - 1);
            this->ship->setYpos(this->ship->getYpos() + 0);
            break;

        case SOUTHWEST:
            cout << " SOUTHWEST\n";
            if ((this->ship->getXpos() == 0 && this->ship->getYpos() % 2 != 0) || this->ship->getYpos() == gridRows)
                return;
            if (this->ship->getYpos() % 2 != 0)
                this->ship->setXpos(this->ship->getXpos() - 1);
            this->ship->setYpos(this->ship->getYpos() + 1);
            break;

        case SOUTHEAST:
            cout << " SOUTHEAST\n";

            if ((this->ship->getXpos() == gridCols && this->ship->getYpos() % 2 == 0) || this->ship->getYpos() == gridRows)
                return;
            if (this->ship->getYpos() % 2 == 0)
                this->ship->setXpos(this->ship->getXpos() + 1);
            this->ship->setYpos(this->ship->getYpos() + 1);
            break;
        }
        sf::Vector3f cubial;
        cout << "( " << cubial.x << ", " << cubial.y << ", " << cubial.z << ") \n";
        cout << "( " << this->ship->getXpos() << ", " << this->ship->getYpos() << " )\n";
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

void DrawShips(sf::RenderWindow & window, HexGrid &grid, vector<DrawShip> & shipList);
DrawShip * GetShipHere(sf::Vector2f pos, vector<DrawShip> & shipList);

// Thread to check for server sending messages
void checkerThread()
{
    // Buffer for the message incoming
    char receivedMessage[1500];
    //char * receivedMessage;
    while (1)
    {
        // Clear buffer
        memset(receivedMessage, 0, sizeof(receivedMessage));

        // Try to receive a message from the server
        recv(clientSd, (char*)&receivedMessage, sizeof(receivedMessage), 0);

        if(!strcmp(receivedMessage, "exit"))
        {
            cout << "Server has quit the session" << endl;
            exit(0);
        }

        int size = 0;
        for(int i = 0; receivedMessage[i] != '\0'; i++)
            size +=1;

        std::vector<Ship*> resultShips = Protocol::ParseShipMessage(clientSd, receivedMessage, size);
        cerr << "Size: " << size << endl;
        cerr << "We got " << resultShips.size() << " ships breh\n"; 
       /* // Print for the client number and the message sent
        cout << "Client " << receivedMessage[0] << ": ";
        for(int i = 1; receivedMessage[i] != '\0'; i++)
        {
            cout << receivedMessage[i];
        }
        cout << endl;*/
    }
}

int main(int argc, char *argv[])
{
    using namespace Protocol;
    // Client-server stuff

    // For the messages to the server upon key presses


//    char msg[5000];

    // Usage
    if(argc != 3)
    {
        cerr << "Usage: ip_address port" << endl; exit(0); 
    } 

    // Gets the IP and Port
    char *serverIp = argv[1]; 
    int port = atoi(argv[2]);

    // Setup a socket and connection tools 
    struct hostent* host = gethostbyname(serverIp); 
    sockaddr_in sendSockAddr;   
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 
    sendSockAddr.sin_family = AF_INET; 
    sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    sendSockAddr.sin_port = htons(port);
    clientSd = socket(AF_INET, SOCK_STREAM, 0);

    // Try to connect
    int status = connect(clientSd, (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));

    if(status < 0)
    {
        cout << "Error connecting to server!" << endl;
        exit(0);
    }
    else
        cout << "Connected to the server!" << endl;

    // Spawn the thread to check for incoming messages from the server
    thread t1(checkerThread);
    // window logic
    sf::RenderWindow window(sf::VideoMode(1270, 720), "Starfinder Commander");
    window.setFramerateLimit(60);
    sf::Vector2f origin;
    origin.x = 100;
    origin.y = 100;
    HexGrid grid(0, 0, 100, 100, 20, sf::LinesStrip);

    sf::VertexArray testGrid;
    testGrid.setPrimitiveType(grid.GetPrimitiveType());
    testGrid = grid.GenerateHexGrid();

    bool leftMouseDragging = false;
    bool potentialDoubleLeftClick = false;
    sf::View camera = window.getView();
    sf::View hud = sf::View(window.getView());

    sf::Vector2u winSize = window.getSize();
    //sf::RectangleShape hudRect = sf::RectangleShape(sf::Vector2f(0, 670));
    //hudRect.setSize(sf::Vector2f(1270, 50));
    sf::Vector2f mPos_old = window.getView().getCenter();
    
    bool shipSelected;
    Ship * selectedShip = NULL;
    int clientShip = 0; // index for this client's DrawShip in ships vector

    sf::Font font;
    if (!font.loadFromFile("testFont.ttf")) {
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

    // Game logic
    vector<DrawShip> drawShips;
    sf::Clock inputDelayTimer;

#pragma region GameLogic
    DrawShip ds;
    Ship* testShip = new Ship();
    testShip->setArmourClass(20);
    testShip->setTargetLock(10);
    testShip->setHullPointsMax(100);
    testShip->setHullPointsCur(53);
    testShip->setShieldMax(Shield::Fore, 77);
    testShip->setShieldMax(Shield::Aft, 78);
    testShip->setShieldMax(Shield::Port, 79);
    testShip->setShieldMax(Shield::Starboard, 90);
    testShip->setShieldCur(Shield::Fore, 91);
    testShip->setShieldCur(Shield::Aft, 92);
    testShip->setShieldCur(Shield::Port, 93);
    testShip->setShieldCur(Shield::Starboard, 95);
/*
*/
    sf::Sprite* testSprite = new sf::Sprite();
    sf::Texture testTex;
    std::vector<Ship*> ships;
    if (testTex.loadFromFile("./images/Sprite1ENG_ON.png"/*, sf::IntRect(10, 10, 32, 34)*/))
    {
        
        testSprite->setTexture(testTex);
        testSprite->setScale(0.4, 0.4);
        sf::Vector2f testOrigin = testSprite->getOrigin();
        testOrigin.x = testSprite->getOrigin().x + (testSprite->getLocalBounds().width / 2);
        testOrigin.y = testSprite->getOrigin().y + (testSprite->getLocalBounds().height / 2);
        testSprite->setOrigin(testOrigin);
        testOrigin = sf::Vector2f(testSprite->getLocalBounds().height, testSprite->getLocalBounds().width);


        ds.setSprite(testSprite);
        ds.setShip(testShip);
        drawShips.push_back(ds);
        ships.push_back(drawShips.at(0).getShip());
        cout << ships[0]->toString() << endl;
        int message_size;
        char* testSerialization = Protocol::CrunchetizeMeCapn(ships, message_size);

        //for(int i = 0; i < message_size; i++)
        //    printf("%x ", testSerialization[i]);
        //printf("\n");

       // cerr << "testSerialization: " << message_size << endl;
        std::vector<Ship*> deserializedShips;
        deserializedShips = Protocol::ParseShipMessage(clientSd, testSerialization, message_size); 
        cout << deserializedShips[0]->toString() << endl;

        cerr << "Waah";
        return 0;
       /* 
        */
    }
    sf::CircleShape selector(20, 6);
    sf::Vector2f selectorPosition = selector.getOrigin();

    selectorPosition.x = selector.getOrigin().x + (selector.getLocalBounds().height / 2);
    selectorPosition.y = selector.getOrigin().y + (selector.getLocalBounds().height / 2);

    selector.setOrigin(selectorPosition);
    selector.setFillColor(sf::Color(0, 0, 255, 100));

    selector.setPosition(sf::Vector2f(grid.offset_to_pixel(sf::Vector2f(99, 99))));

    sf::CircleShape selectedShipOverlay(20,6);
    sf::Vector2f selectedShipPosition = selectedShipOverlay.getOrigin();

    selectedShipPosition.x = selectedShipOverlay.getOrigin().x + (selectedShipOverlay.getLocalBounds().height / 2);
    selectedShipPosition.y = selectedShipOverlay.getOrigin().y + (selectedShipOverlay.getLocalBounds().height / 2);

    selectedShipOverlay.setOrigin(selectedShipPosition);
    selectedShipOverlay.setFillColor(sf::Color(255, 255, 0, 50));

    selectedShipOverlay.setPosition(sf::Vector2f(grid.offset_to_pixel(sf::Vector2f(99,99))));

#pragma endregion
    float delt;
    int clickCount = 0;
    bool rotated = false;
    sf::Vector2f mousePos;

    winSize = window.getSize();
    //hudRect.setPosition(sf::Vector2f(50, 50));
    //hudRect.setSize(sf::Vector2f(30, 50));
    sf::Event event;
    inputDelayTimer.restart();
    while (window.isOpen())
    {
       // if(inputDelayTimer.getElapsedTime().asMilliseconds() < INPUT_DELAY)
       //     continue;
       // inputDelayTimer.restart();
        
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

                    cout << "( " << event.mouseButton.x << ", " << event.mouseButton.y << " )\n";

                    // left button down
                    leftMouseDragging = true;
                    mPos_old = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    leftDragTimer.restart();
                }
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    // right button down, set highlight selector
                    sf::Vector2f clickPosition = grid.pixel_to_offset(sf::Vector2f(mousePos.x, mousePos.y));
                    cout << "Click at : " << clickPosition.x << ", " << clickPosition.y << endl;
                    selector.setPosition(grid.offset_to_pixel(clickPosition));
                    // check that grid for a ship at the new position
                    DrawShip* shipHere = GetShipHere(clickPosition, drawShips);
                    if(shipHere != NULL)
                    {
                        if(shipSelected == false)
                        {
                            cout << "Ship here: \n" << shipHere->getShip()->toString();
                            selectedShipOverlay.setPosition(grid.offset_to_pixel(clickPosition));
                            selector.setPosition(sf::Vector2f(grid.offset_to_pixel(sf::Vector2f(99,99))));
                            shipSelected = true;
                            selectedShip = shipHere->getShip();
                        }
                        else
                        {
                            cout << "Deselecting ship\n";
                            shipSelected = false;
                            selectedShip = NULL;
                            selectedShipOverlay.setPosition(grid.offset_to_pixel(sf::Vector2f(99,99)));
                        }
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
                // 80 - 5000
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
                //hudRect.setSize(sf::Vector2f(event.size.width-20, 50));
                //hudRect.setPosition(sf::Vector2f(10, event.size.height - 100));

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
                drawShips[0].Left();

            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                moved = true;
                drawShips[0].Right();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                moved = true;
                drawShips[0].Forward(grid);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                moved = true;
                drawShips[0].Back(grid);
            }

            if(moved)
            {
//                memset(&msg, 0, sizeof(msg));
  //              strcpy(msg, "Down");
    //            send(clientSd, (char*)&msg, strlen(msg), 0);
                
                int message_length;
                char * message = Protocol::CrunchetizeMeCapn(ships, message_length); 
                send(clientSd, message, message_length, 0);
            }
            
            if(shipSelected)
            {
            selectedShipOverlay.setPosition(sf::Vector2f(grid.offset_to_pixel( drawShips[clientShip].position() )));     
            }
        }

#pragma endregion

        window.clear();
        
        window.setView(camera);
        window.draw(testGrid);
        DrawShips(window, grid, drawShips);
        window.draw(selector);
        window.draw(selectedShipOverlay);

        window.setView(hud);
        //window.draw(hudRect);
        window.draw(hudText);
        window.display();
        window.setView(camera);
	}

    t1.join();
    close(clientSd);
	return 0;
}

void DrawShips(sf::RenderWindow &window, HexGrid &grid, vector<DrawShip> & shipList)
{
    for (int i = 0; i < shipList.size(); i++)
    {
        shipList[i].Draw(window, grid);
    }
}

DrawShip * GetShipHere(sf::Vector2f pos, vector<DrawShip> & shipList)
{
    for(int i = 0; i < shipList.size(); i++)
    {
        if(shipList[i].getXpos() == pos.x && shipList[i].getYpos() == pos.y)
            return &shipList[i];
    }
    return NULL;
}