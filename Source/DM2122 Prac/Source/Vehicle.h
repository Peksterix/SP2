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

	Vector3 pos;
	Vector3 rotation;

public:
	std::vector<Position> wheelPos, weaponPos, wheelScale, weaponScale;
	Position position;
	Position rotate;
	Position scale;
	physics RB;

	float maxSpeed;
	float accel;
	
	float vehTurningSpeed;
	float turningAngle;
	//set to vehicle
	float turningSpeed;
	float maxTurningSpeed;

	float cMaxTurningSpeed;
	float cTurningSpeedRate;
	float cTurningSpeedDecayRate;

	physics* getRB();
	void updatePos();
	Vehicle getFront();
	void setFront();

	Vehicle();
	~Vehicle();

	Chassis* getChassis();
	Wheel* getWheel();
	Weapon* getWeapon();
	std::string getName();
	Vector3 getPos();
	Vector3 getRotation();
	void setPos(float x, float y, float z);
	void setRotation(Vector3 rotation);

	void setChassis(int Type);
	void setWheel(int Type);
	void setWeapon(int Type);
	void setName(std::string string);
	
	void setTurningSpeed(float speed);
	void setMaxTurnSpeed(float speed);
	float getTurnRate();

	void Update();
};

#endif