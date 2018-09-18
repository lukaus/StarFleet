#include <vector>
#include <cstring>
#include "Ship.h"
#include "Projectile.h"

using namespace std;

#ifndef PROTOCOL_H
#define PROTOCOL_H
namespace Protocol
{
	int	ParseClientIDMessage(char * message, int message_size);
    std::vector<Ship*> ParseShipMessage(int sd, char * message, int message_size, int &clientID);
    char* CrunchetizeMeCapn(int clientID, std::vector<Ship*> shipArr, int& message_size);
    std::vector<Projectile*> ParseProjectileMessage(char* message);
    char* SerializeProjectileArray(std::vector<Projectile*> projArr);

}
#endif