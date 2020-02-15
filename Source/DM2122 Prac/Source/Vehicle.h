#ifndef VEHICLE_H
#define VEHICLE_H

#include "Object.h"

class Vehicle: public Object
{
private:
	float bbScale;

public:
	Vehicle();
	~Vehicle();

	float getBBScale();
};

#endif