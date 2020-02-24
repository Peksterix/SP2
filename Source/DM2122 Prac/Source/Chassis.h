#ifndef CHASSIS_H
#define CHASSIS_H

#include "Entity.h"
#include "Mesh.h"
#include <vector>

class Chassis: public Entity
{
private:
	int health,
		type;

	float bbScale;

	std::string name;
	//Mesh* mesh;

public:
	std::vector<Position> wheelPos, weaponPos, wheelScale, weaponScale;

	Chassis(int Type);
	~Chassis();

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