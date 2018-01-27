#include "Ship.h"


int		Ship::getCost()
{
	return cost;
}
void 	Ship::setCost(int c)
{
	cost = c;
}
int 	Ship::calculateCost(float)
{
	return cost = -1;
}

int 	Ship::getMinCrew()
{
	return minCrew;
}
void 	Ship::setMinCrew(int m)
{
	minCrew = m;
}

int 	Ship::getMaxCrew()
{
	return maxCrew;
}
void 	Ship::setMaxCrew(int m)
{
	maxCrew = m;
}

int 	Ship::setModifier(Modifier mod, bool val)
{
	modifiers[mod] = val;
    return 0;
}
bool 	Ship::getModifierIsActive(Modifier mod)
{
    return (modifiers[mod]);
}
int* 	Ship::getModifiers()
{
    return modifiers;
}

int 	Ship::getHullPointsMax()
{
    return hullPointsMax;
}
void 	Ship::setHullPointsMax(int n)
{
    hullPointsMax = n;
}

int 	Ship::getPowerCoreTotal()
{
    return powerCoreTot;
}					

void 	Ship::setPowerCoreTotal(int val)
{
    powerCoreTot = val;
}

Crewman* 	Ship::getCrewman(Station station)
{
    return crewmen[station];
}
void 	Ship::assignCrewman(Crewman* crew, Station stat)
{
    crewmen[stat] = crew;
}

string 	Ship::getName()
{
    return name;
}
void 	Ship::setName(string n)
{
    name = n;
}

float 	Ship::getTier()
{
    return tier;
}
void 	Ship::setTier(float f)
{
    tier = f;
}

int 	Ship::getSize()
{
    return size;
}
void 	Ship::setSize(int s)
{
    size = s;
}

int 	Ship::getSpeed()
{
    return speed;
}
void 	Ship::setSpeed(int val)
{
    speed = val;
}

Maneuverability 	Ship::getManeuverability()
{
    return maneuv;
}
void 	Ship::setManeuverability(Maneuverability m)
{
    maneuv = m;
}

float 	Ship::getDriftRating()
{
    return driftRating;
}
void 	Ship::setDriftRating(float d)
{
    driftRating = d;
}

int 	Ship::getArmourClass()
{
    return armourClass;
}
void 	Ship::setArmourClass(int ac)
{
    armourClass = ac;
}

int 	Ship::getTargetLock()
{
    return targetLock;
}
void 	Ship::setTargetLock(int tl)
{
    targetLock = tl;
}

int 	Ship::getDamageThreshold()
{
    return damageThreshold;
}
void 	Ship::setDamageThreshold(int dt)
{
    damageThreshold = dt;
}

string 	Ship::getSystems()
{
    string all_systems = "";
    for(int i = 0; i < systems->length(); i++)
    {
        all_systems += systems[i] + "\n";
    }
    return all_systems;
}
void 	Ship::addSystem(string s)
{
    systems->append(s);
}

string 	Ship::getExpansionBays()
{
    string all_expansions = "";
    for (int i = 0; i < expansionBays->length(); i++)
    {
        all_expansions += expansionBays[i] + "\n";
    }
    return all_expansions;
}
void 	Ship::addExpansionBay(string s)
{
    expansionBays->append(s);
}


int     Ship::getXpos()
{
    return x_pos;
}
void    Ship::setXpos(int x) 
{
    x_pos = x;
}

int     Ship::getYpos() 
{
    return y_pos;
}
void    Ship::setYpos(int y) 
{
    y_pos = y;
}

Orientation Ship::getOrientation() 
{
    return orientation;
}
void        Ship::setOrientation(Orientation o) 
{
    orientation = o;
}