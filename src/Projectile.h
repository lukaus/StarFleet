#include "Ship.h"

#ifndef PROJECTILE_H
#define PROJECTILE_H

enum POrientation : int
{
    PEAST,
    PSOUTHEAST,
    PSOUTHWEST,
    PWEST,
    PNORTHWEST,
    PNORTHEAST
};

class Projectile
{
private:
    int roll;
    int x_pos;
    int y_pos;
    int damage;
	 POrientation orientation; 

    float speed; // if we can, nbd tho
public:
   Projectile();
   Projectile(int roll, int x_pos, int y_pos, POrientation orientation);
   // accessors and mutators here
   int getRoll();
   void setRoll(int);

   int getX_pos();
   void setX_pos(int);

   int getY_pos();
   void setY_pos(int);

   int getDamage();
   void setDamage(int);

   POrientation getOrientation();
   void setOrientation(POrientation o);
};

#endif
