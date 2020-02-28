#ifndef WEAPON_H
#define WEAPON_H

#include "Entity.h"
#include "Bullet.h"
#include <vector>

class Weapon : public Entity
{
private:
	int health,
		magSize,
		type;

	float	mass,
			fireRate,
			damage;

	std::string name;

public:
	Weapon(int Type);
	~Weapon();

	int getHealth();
	int getType();
	float getMass();
	float getBBScale();
	std::string getName();
	Mesh* getMesh();
	float getDamage();
	float getMag();
	float getFire();

	void setHealth(int Health);
	void setType(int Type);
	void setBBScale(int BBScale);
	void setMag(int mag);
};

#endif