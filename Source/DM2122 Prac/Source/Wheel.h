#ifndef WHEEL_H
#define WHEEL_H

#include "Object.h"
#include "Mesh.h"

class Wheel : public Object
{
private:
	int health,
		type;

	float bbScale;

	Mesh* mesh;

public:
	Wheel(int Type);
	~Wheel();

	int getHealth();
	int getType();
	float getBBScale();
	Mesh* getMesh();

	void setHealth(int Health);
	void setType(int Type);
	void setBBScale(int BBScale);
};

#endif