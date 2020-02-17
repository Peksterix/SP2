#ifndef WEAPON_H
#define WEAPON_H

#include "Object.h"
#include "Mesh.h"

class Weapon : public Object
{
private:
	int health,
		type;

	float bbScale;

	Mesh* mesh;

public:
	Weapon(int Type);
	~Weapon();

	int getHealth();
	int getType();
	float getBBScale();
	Mesh* getMesh();

	void setHealth(int Health);
	void setType(int Type);
	void setBBScale(int BBScale);
};

#endif