#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include "Ship.h"
#include "Projectile.h"
#include "Protocol.h"

using namespace std;

#define SHIP_INTS (17) // up this when stuff added

int Protocol::ParseClientIDMessage(char * message, int message_size)
{
    int message_index = 0;
    char message_type = 'Z';
    memcpy(&message_type, &message[message_index], sizeof(char));
    message_index += sizeof(char);

    int cid = -1;
    memcpy(&cid, &message[message_index], sizeof(int));
    message_index += sizeof(int);
    return cid;
}

std::vector<Ship*> Protocol::ParseShipMessage(int sd, char * message, int message_size, int &clientID)
{
    std::vector<Ship*> shipArray;
    int message_index = 0;
    char message_type = 'Z';
    memcpy(&message_type, &message[message_index], sizeof(char));
    message_index += sizeof(char);

    int message_length = 0;
    memcpy(&message_length, &message[message_index], sizeof(int));
    message_index += sizeof(int);

    int cid = -1;
    memcpy(&cid, &message[message_index], sizeof(int));
    message_index += sizeof(int);
    clientID = cid;

    int numberOfShips = 1;
    memcpy(&numberOfShips, &message[message_index], sizeof(int));
    message_index += sizeof(int);
    
    for(int i = 0; i < numberOfShips; i++)
    {
        Ship* thisShip = new Ship();

        // 0:   ID
        int val;
        memcpy(&val, &message[message_index], sizeof(int));
        message_index += sizeof(int);
        thisShip->setID(val);
    
        // 1:   X pos
        memcpy(&val, &message[message_index], sizeof(int));
        message_index += sizeof(int);
        thisShip->setXpos(val);

        // 2:   Y pos
        memcpy(&val, &message[message_index], sizeof(int));
        message_index += sizeof(int);
        thisShip->setYpos(val);

        // 3:  orientation 
        memcpy(&val, &message[message_index], sizeof(int));
        message_index += sizeof(int);
        thisShip->setOrientation( (Orientation) val);

        // 4:   HP cur
        memcpy(&val, &message[message_index], sizeof(int));
        message_index += sizeof(int);
        thisShip->setHullPointsCur(val);

        // 5; HP max
        memcpy(&val, &message[message_index], sizeof(int));
        message_index += sizeof(int);
        thisShip->setHullPointsMax(val);

        // 6:   target lock
        memcpy(&val, &message[message_index], sizeof(int));
        message_index += sizeof(int);
        thisShip->setTargetLock(val);

        // 7; Armour Class
        memcpy(&val, &message[message_index], sizeof(int));
        message_index += sizeof(int);
        thisShip->setArmourClass(val);

        // 8:   attack bonus
        memcpy(&val, &message[message_index], sizeof(int));
        message_index += sizeof(int);
        thisShip->setAttackBonus(val); // attack bonus

        for(int j = 0; j < 4; j++)
        {
            memcpy(&val, &message[message_index], sizeof(int));
            message_index += sizeof(int);
            thisShip->setShieldCur((Shield)j, val);

            memcpy(&val, &message[message_index], sizeof(int));
            message_index += sizeof(int);
            thisShip->setShieldMax((Shield)j, val);
        }
        shipArray.push_back(thisShip);

    }
    return shipArray;
}

std::vector<Projectile*> Protocol::ParseProjectileMessage(char* message)
{
    std::vector<Projectile*> projectiles;

    return projectiles;
}

char * Protocol::CrunchetizeMeCapn(int clientID, std::vector<Ship*> shipArr, int &message_size)
{
    message_size = sizeof(char) + sizeof(int) + sizeof(int) + sizeof(int) + (shipArr.size() * (sizeof(int) * SHIP_INTS));
    char* message = new char[message_size];

    char message_type = 'S';
    int message_index = 0;
    int numberOfShips = shipArr.size();

    memcpy(&message[message_index], &message_type, sizeof(char));
    message_index += sizeof(char); // skip to next byte
        
    memcpy(&message[message_index], &message_size, sizeof(int));
    message_index += sizeof(int); // skip to next byte

    memcpy(&message[message_index], &clientID, sizeof(int));
    message_index += sizeof(int); // skip to next byte
  
    memcpy(&message[message_index], &numberOfShips, sizeof(int));
    message_index += sizeof(int); // skip to next byte
    

    // prepare message
    for(int i = 0; i < shipArr.size(); i++)
    {
        // 0:   ID
        int val = shipArr[i]->getID();
        memcpy(&message[message_index], &val, sizeof(int));
        message_index += sizeof(int);
        
        // 1:   X pos
        val = shipArr[i]->getXpos();
        memcpy(&message[message_index], &val, sizeof(int));
        message_index += sizeof(int);
        
        // 2:   y pos
        val = shipArr[i]->getYpos();
        memcpy(&message[message_index], &val, sizeof(int));
        message_index += sizeof(int);
        
        // 3:   orientation
        val = (int)shipArr[i]->getOrientation();
        memcpy(&message[message_index], &val, sizeof(int));
        message_index += sizeof(int);
        
        // 4:   HP (cur)
        val = shipArr[i]->getHullPointsCur();
        memcpy(&message[message_index], &val, sizeof(int));
        message_index += sizeof(int);

        // 5:   HP (tot)
        val = shipArr[i]->getHullPointsMax();
        memcpy(&message[message_index], &val, sizeof(int));
        message_index += sizeof(int);
        
        // 6:   TL
        val = shipArr[i]->getTargetLock();
        memcpy(&message[message_index], &val, sizeof(int));
        message_index += sizeof(int);
       
        // 7:   AC
        val = shipArr[i]->getArmourClass();
        memcpy(&message[message_index], &val, sizeof(int));
        message_index += sizeof(int);
      
        // 8:   attack bonus
        val = (int) 0;
        memcpy(&message[message_index], &val, sizeof(int));
        message_index += sizeof(int);
        
        for(int j = 0; j < 4; j++)
        {
            val = shipArr[i]->getShieldCur((Shield)j);
            memcpy(&message[message_index], &val, sizeof(int));
            message_index += sizeof(int);

            val = shipArr[i]->getShieldMax((Shield)j);
            memcpy(&message[message_index], &val, sizeof(int));
            message_index += sizeof(int);
        }
    }
    return message;
}

char* Protocol::SerializeProjectileArray(std::vector<Projectile*> projArr)
{
    char* message;

    message = new char [1];

    return message;
}
