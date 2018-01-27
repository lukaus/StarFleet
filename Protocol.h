#include <vector>
#include <cstring>
#include "Ship.h"
#include "Projectile.h"

using namespace std;

#ifndef PROTOCOL_H
#define PROTOCOL_H
namespace Protocol
{

    static std::vector<Ship*> ParseShipMessage(char * message, int message_size);
    static char* CrunchetizeMeCapn(std::vector<Ship*> shipArr, int& message_size);
    static std::vector<Projectile*> ParseProjectileMessage(char* message);
    static char* SerializeProjectileArray(std::vector<Projectile*> projArr);

}
#endif