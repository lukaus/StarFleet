#include <vector>
#include <cstring>
#include "Ship.h"
#include "Projectile.h"

using namespace std;

#ifndef PROTOCOL_H
#define PROTOCOL_H
namespace Protocol
{

    std::vector<Ship*> ParseShipMessage(int clientID, char * message, int message_size);
    char* CrunchetizeMeCapn(std::vector<Ship*> shipArr, int& message_size);
    std::vector<Projectile*> ParseProjectileMessage(char* message);
    char* SerializeProjectileArray(std::vector<Projectile*> projArr);

}
#endif