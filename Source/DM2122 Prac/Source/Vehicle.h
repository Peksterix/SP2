#ifndef VEHICLE_H
#define VEHICLE_H

#include "Entity.h"
#include "Chassis.h"
#include "Weapon.h"
#include "Wheel.h"
#include <vector>

class Vehicle: public Entity
{
private:

	std::string name;

	Chassis* chassis;
	Wheel* wheel;
	Weapon* weapon;

public:
	std::vector<Position> wheelPos, weaponPos, wheelScale, weaponScale;

	Vehicle();
	~Vehicle();

	Chassis* getChassis();
	Wheel* getWheel();
	Weapon* getWeapon();
	std::string getName();

	void setChassis(int Type);
	void setWheel(int Type);
	void setWeapon(int Type);
	void setName(std::string string);

	void Update();
};

#endif