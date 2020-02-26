#ifndef VEHICLE_H
#define VEHICLE_H

#include "Entity.h"
#include "Chassis.h"
#include "Weapon.h"
#include "Wheel.h"
#include <vector>
#include "Physics.h"

class Vehicle: public Entity
{
private:

	std::string name;

	Chassis* chassis;
	Wheel* wheel;
	Weapon* weapon;

	Vector3 rotation;

public:
	std::vector<Position> wheelPos, weaponPos, wheelScale, weaponScale;
	physics RB;

	float maxSpeed;

	float vehTurningSpeed;
	float turningAngle;
	//set to vehicle
	float turningSpeed;
	float maxTurningSpeed;

	float cMaxTurningSpeed;
	float cTurningSpeedRate;
	//float cTurningSpeedDecayRate;

	physics* getRB();
	void updatePos();
	Vehicle getFront();
	void setFront();
	void setTurningSpeed(float speed);
	void setMaxTurnSpeed(float speed);
	float getTurnRate();

	Vector3 getRotation();
	void setRotation(Vector3 rotation);

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