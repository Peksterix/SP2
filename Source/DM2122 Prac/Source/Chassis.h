#ifndef CHASSIS_H
#define CHASSIS_H

#include "Object.h"
#include "Mesh.h"

class Chassis: public Object
{
private:
	int health,
		type;

	float bbScale;

	Mesh* mesh;
	Position wheelPos[4], weaponPos;

public:
	Chassis(int Type);
	~Chassis();

	int getHealth();
	int getType();
	float getBBScale();
	Mesh* getMesh();

	void setHealth(int Health);
	void setType(int Type);
	void setBBScale(int BBScale);
};

#endif