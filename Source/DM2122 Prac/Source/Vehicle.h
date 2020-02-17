#ifndef VEHICLE_H
#define VEHICLE_H

#include "Object.h"
#include "Chassis.h"
#include "Weapon.h"
#include "Wheel.h"

class Vehicle: public Object
{
private:

	Chassis* chassis;
	Wheel* wheel;
	Weapon* weapon;

public:
	Vehicle();
	~Vehicle();

	Chassis* getChassis();
	Wheel* getWheel();
	Weapon* getWeapon();

	void setChassis(int Type);
	void setWheel(int Type);
	void setWeapon(int Type);

	void Update();
};

#endif