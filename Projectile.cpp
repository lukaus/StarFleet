#include "Projectile.h"

Projectile::Projectile()
{
    x_pos = 0;
    y_pos = 0;
}

Projectile::Projectile(int roll, int x_pos, int y_pos, Orientation orientation)
{
}
// mutators and accessors
int Projectile::getRoll()
{
    return roll;
}
void Projectile::setRoll(int r)
{
    roll = r;
}

int Projectile::getX_pos()
{
    return x_pos;
}
void Projectile::setX_pos(int x)
{
    x_pos = x;
}

int Projectile::getY_pos()
{
    return y_pos;
}
void Projectile::setY_pos(int y)
{
    y_pos = y;
}

int Projectile::getDamage()
{
    return damage;

}
void Projectile::setDamage(int d)
{
    damage = d;
}

Orientatation Projectile::getOrientation()
{
    return orientation;
}
void Projectile::setOrientation(Orientation o)
{
    orientation = o;
}
