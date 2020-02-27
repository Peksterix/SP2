#ifndef WHEEL_H
#define WHEEL_H

#include "Entity.h"
#include "Mesh.h"

class Wheel : public Entity
{
private:
	int health,
		type;

	float	friction,
			speed;

	std::string name;
	//Mesh* mesh;

public:
	Wheel(int Type);
	~Wheel();

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