#ifndef PROJECTILE_H
#define PROJECTILE_H

enum Orientation : int
{
    EAST,
    SOUTHEAST,
    SOUTHWEST,
    WEST,
    NORTHWEST,
    NORTHEAST
};

class Projectile
{
private:
    int roll;
    int x_pos;
    int y_pos;
    int damage;
	Orientation orientation; 

    float speed; // if we can, nbd tho
public:
   Projectile();
   Projectile(int roll, int x_pos, int y_pos, Orientation orientation);
   // accessors and mutators here
   int getRoll();
   void setRoll(int);

   int getX_pos();
   void setX_pos(int);

   int getY_pos();
   void setY_pos(int);

   int getDamage();
   void setDamage(int);

   Orientation getOrientation();
   void setOrientaiton(Orientation);
};

#endif
