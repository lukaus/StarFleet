#include <vector>
#include <string>
#include "Ship.h"
#include "Projectile.h"

using namespace std;

#ifndef PROTOCOL_H
#define PROTOCOL_H



std::vector<Ship> ParseShipMessage(char *);
std::vector<Projectile> ParseProjectileMessage(char* );
char* SerializeShipArray(std::vector<Ship>);
char* SerializerProjectileArray(std::vector<Projectile>);



#endif