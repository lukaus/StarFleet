#include <vector>
#include <string>
#include "Ship.h"
#include "Projectile.h"
#define SHIP_INTS (15)

using namespace std;

std::vector<Ship> ParseShipMessage(char * message)
{
    std::vector<Ship> shipArray;

    int currentOffset = 4;
    while(currentOffset < sizeof(message))
    {


    }

    return shipArray;
}

std::vector<Projectile> ParseProjectileMessage(char* message)
{

}

char* SerializeShipArray(std::vector<Ship> shipArr)
{
    int message_size = 5 + (shipArr.size() * (sizeof(int) * SHIP_INTS));
    char* message;
    message = new char [message_size];
//    for(int i = 0; i < message_size; i++)
//        message[i] = new char*;
    message[0] = 'S'; // sign message as a ship array
    message[1] = message_size;

    int message_index = 1 + sizeof(int); // skip to next byte

    // prepare message
    for(int i = 0; i < shipArr.size(); i++)
    {
        // each ship, send:
        // 16:   x pos
        message[message_index] = shipArr[i].getXpos();
        message_index += sizeof(int);
        // 16:   y pos
        message[message_index] = shipArr[i].getYpos();
        message_index += sizeof(int);
        // 16:   HP (cur)
        message[message_index] = shipArr[i].getHullPointsCur();
        message_index += sizeof(int);
        // 16:   HP (tot)
        message[message_index] = shipArr[i].getHullPointsMax();
        message_index += sizeof(int);
        // 16:   TL
        message[message_index] = shipArr[i].getTargetLock();
        message_index += sizeof(int);
        // 16:   AC
        message[message_index] = shipArr[i].getArmourClass();
        message_index += sizeof(int);
        // 16:   attack bonus
        message[message_index] = (int) 0;
        message_index += sizeof(int);
        // 16*4: current shield values
        for(int j = 0; j < 4; j++)
        {
            message[message_index] = shipArr[i].getShieldCur(Shield::Fore);
            message_index += sizeof(int);
        }
        // 16*4: total shield values
        for(int j = 0; j < 4; j++)
        {
            message[message_index] = shipArr[i].getShieldMax(Shield::Fore);
            message_index += sizeof(int);
        }
    }
}

char* SerializeProjectileArray(std::vector<Projectile> projArr)
{

}
