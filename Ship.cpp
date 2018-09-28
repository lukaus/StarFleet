#include "Ship.h"

using std::to_string;

        Ship::Ship()
{
    id = -1;
    x_pos = 0;
    y_pos = 0;
}

        Ship::Ship(int sid, int sp, Maneuverability m, int ac, int tl, int dm, int crit, int pcc, int hp, int * shield)
{
    id = sid;

    x_pos = 0;
    y_pos = 0;

    speed = sp;
    maneuv = m;
    armourClass = ac;
    targetLock = tl;
    damageThreshold = dm;
    criticalThreshold = crit;
    powerCoreTot = pcc;
    powerCoreAvl = pcc;
    hullPointsMax = hp;
    hullPointsCur = hp;

    for (int i = 0; i < 4; ++i)
    {
        shieldTot[i] = shield[i];
        shieldCur[i] = shield[i];
    }
}

int Ship::getID()
{
    return id;
}

void Ship::setID(int sid)
{
    id = sid;
}

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

int     Ship::getHullPointsMax()
{
    return hullPointsMax;
}
void    Ship::setHullPointsMax(int n)
{
    hullPointsMax = n;
}

int     Ship::getHullPointsCur()
{
    return hullPointsCur;
}
void    Ship::setHullPointsCur(int n)
{
    hullPointsCur = n;
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

int         Ship::getShieldMax(Shield sh)
{
    return shieldTot[sh];
}

void        Ship::setShieldMax(Shield sh, int val)
{
    shieldTot[sh] = val;
}

int         Ship::getShieldCur(Shield sh)
{
    return shieldCur[sh];
}

void        Ship::setShieldCur(Shield sh, int val)
{
    shieldCur[sh] = val;
}



string    Ship::toString()
{
    string toReturn = "";
    toReturn += "X:"; 
    toReturn += to_string(x_pos);
    toReturn += " Y:"; 
    toReturn += to_string(y_pos);
    toReturn += " O:"; 
    toReturn += to_string(orientation);
    toReturn += " AC:"; 
    toReturn += to_string(armourClass);
    toReturn += " TL:"; 
    toReturn += to_string(targetLock);
    toReturn += " HPc:"; 
    toReturn += to_string(hullPointsMax);
    toReturn += " HPm:"; 
    toReturn += to_string(hullPointsCur);
    toReturn += " SFM:"; 
    toReturn += to_string(shieldTot[Shield::Fore]);
    toReturn += " SAM:"; 
    toReturn += to_string(shieldTot[Shield::Aft]);
    toReturn += " SPM:"; 
    toReturn += to_string(shieldTot[Shield::Port]);
    toReturn += " SSM:"; 
    toReturn += to_string(shieldTot[Shield::Starboard]);
    toReturn += " SFC:"; 
    toReturn += to_string(shieldCur[Shield::Fore]);
    toReturn += " SAC:"; 
    toReturn += to_string(shieldCur[Shield::Aft]);
    toReturn += " SPC:"; 
    toReturn += to_string(shieldCur[Shield::Port]);
    toReturn += " SSC:"; 
    toReturn += to_string(shieldCur[Shield::Starboard]);

    return toReturn;

}
Ship& Ship::operator= (const Ship &ship)
{
    // do the copy
    x_pos = ship.x_pos;
    y_pos = ship.y_pos;
    targetLock = ship.targetLock;
    armourClass = ship.armourClass;
    hullPointsMax = ship.hullPointsMax;
    hullPointsCur = ship.hullPointsCur;

    for (int i = 0; i < 4; i++)
    {
    	shieldTot[i] = ship.shieldTot[i];
    	shieldCur[i] = ship.shieldCur[i];
    }
 
    // return the existing object so we can chain this operator
    return *this;
}
