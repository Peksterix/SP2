#ifndef WEAPON_H
#define WEAPON_H

#include "Entity.h"
#include "Mesh.h"

class Weapon : public Entity
{
private:
	int health,
		type;

	float	ammoType,
			fireRate,
			damage,
			bbScale;

	std::string name;

	Mesh* mesh;

public:
	Weapon(int Type);
	~Weapon();

	int getHealth();
	int getType();
	float getBBScale();
	std::string getName();
	Mesh* getMesh();

	void setHealth(int Health);
	void setType(int Type);
	void setBBScale(int BBScale);
};

#endif