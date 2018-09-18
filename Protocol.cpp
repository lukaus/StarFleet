#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include "Ship.h"
#include "Projectile.h"
#include "Protocol.h"

using namespace std;

#define SHIP_INTS (16) // up this when stuff added

int Protocol::ParseClientIDMessage(char * message, int message_size)
{
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

        int val;
        memcpy(&val, &message[message_index], sizeof(int));
        message_index += sizeof(int);
        thisShip->setXpos(val);

        memcpy(&val, &message[message_index], sizeof(int));
        message_index += sizeof(int);
        thisShip->setYpos(val);

        memcpy(&val, &message[message_index], sizeof(int));
        message_index += sizeof(int);
        thisShip->setOrientation( (Orientation) val);

        memcpy(&val, &message[message_index], sizeof(int));
        message_index += sizeof(int);
        thisShip->setHullPointsCur(val);

        memcpy(&val, &message[message_index], sizeof(int));
        message_index += sizeof(int);
        thisShip->setHullPointsMax(val);

        memcpy(&val, &message[message_index], sizeof(int));
        message_index += sizeof(int);
        thisShip->setTargetLock(val);

        memcpy(&val, &message[message_index], sizeof(int));
        message_index += sizeof(int);
        thisShip->setArmourClass(val);

        memcpy(&val, &message[message_index], sizeof(int));
        message_index += sizeof(int);
       // thisShip->setXpos(val); // attack bonus

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
    //cerr << "Serialization complete, " << shipArray.size() << " ships.\n";
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
    //cerr << "message_size: " << message_size << endl;
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
  
    //cerr << "NumShips index, storing: " << message_index << endl;  
    memcpy(&message[message_index], &numberOfShips, sizeof(int));
    message_index += sizeof(int); // skip to next byte
    

    // prepare message
    for(int i = 0; i < shipArr.size(); i++)
    {
        int val = shipArr[i]->getXpos();
        memcpy(&message[message_index], &val, sizeof(int));
        message_index += sizeof(int);
        
        // 16:   y pos
        val = shipArr[i]->getYpos();
        memcpy(&message[message_index], &val, sizeof(int));
        message_index += sizeof(int);
        
        // 16:   orientation
        val = (int)shipArr[i]->getOrientation();
        memcpy(&message[message_index], &val, sizeof(int));
        message_index += sizeof(int);
        
        // 16:   HP (cur)
        val = shipArr[i]->getHullPointsCur();
        memcpy(&message[message_index], &val, sizeof(int));
        message_index += sizeof(int);
        // 16:   HP (tot)
        
        val = shipArr[i]->getHullPointsMax();
        memcpy(&message[message_index], &val, sizeof(int));
        message_index += sizeof(int);
        
        // 16:   TL
        val = shipArr[i]->getTargetLock();
        memcpy(&message[message_index], &val, sizeof(int));
        message_index += sizeof(int);
       
        // 16:   AC
        val = shipArr[i]->getArmourClass();
        memcpy(&message[message_index], &val, sizeof(int));
        message_index += sizeof(int);
      
        // 16:   attack bonus
        val = (int) 0;
        memcpy(&message[message_index], &val, sizeof(int));
        message_index += sizeof(int);
        // 16*4: current shield values
        
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
