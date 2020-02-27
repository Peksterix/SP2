#include "Vehicle.h"

Vehicle::Vehicle()
{
	name = "N/A";

	chassis = nullptr;
	wheel = nullptr;
	weapon = nullptr;

	maxSpeed = 150.f;
	//accel = 0.6f;

	vehTurningSpeed = 0.f;
	cMaxTurningSpeed = 2.f;
	cTurningSpeedRate = 0.2f;
	cTurningSpeedDecayRate = 10.f;

	RB.setAccel(1.5);
}

Vehicle::~Vehicle()
{
	if (weapon != nullptr) delete weapon;
	if (wheel != nullptr) delete wheel;
	if (chassis != nullptr) delete chassis;
}

Chassis* Vehicle::getChassis() 
{
	if (chassis != nullptr) return chassis;
	else return nullptr;
}

Wheel* Vehicle::getWheel() 
{
	if (wheel != nullptr) return wheel;
	else return nullptr;
}

Weapon* Vehicle::getWeapon() 
{
	if (weapon != nullptr) return weapon;
	else return nullptr;
}

std::string Vehicle::getName() { return name; }

void Vehicle::setChassis(int Type)
{
	if (chassis != nullptr) delete chassis;
	if (Type != -1)
	{
		chassis = new Chassis(Type);
		wheelPos.clear(), weaponPos.clear(), wheelScale.clear(), weaponScale.clear();
		chassis->position = position;
		wheelPos = chassis->wheelPos;
		weaponPos = chassis->weaponPos;
		wheelScale = chassis->wheelScale;
		weaponScale = chassis->weaponScale;
	}
	else chassis = nullptr;
}

void Vehicle::setWheel(int Type)
{
	if (wheel != nullptr) delete wheel;
	if (Type != -1)
	{
		wheel = new Wheel(Type);
		wheel->position = position;
	}
	else wheel = nullptr;
}

void Vehicle::setWeapon(int Type)
{
	if (weapon != nullptr) delete weapon;
	if (Type != -1) 
	{
		weapon = new Weapon(Type);
		weapon->position = position;
	}
	else weapon = nullptr;
}

void Vehicle::setName(std::string string)
{
	name = string;
}

//void Vehicle::Update()
//{
//	if (chassis != nullptr)
//	{
//		if (chassis->getHealth() <= 0) delete chassis;
//	}
//
//	if (wheel != nullptr)
//	{
//		if (wheel->getHealth() <= 0) delete wheel;
//	}
//
//	if (weapon != nullptr)
//	{
//		if (weapon ->getHealth() <= 0) delete weapon;
//	}
//
//}

physics* Vehicle::getRB()
{
	return &RB;
}

void Vehicle::updatePos()
{
	position.x += this->RB.getForce().x;
	position.z += this->RB.getForce().z;
	position.y += this->RB.getForce().y;
	
	chassis->position = position;
	wheel->position = position;
	weapon->position = position;

	//RB.Update();
}

void Vehicle::setTurningSpeed(float speed)
{
	turningSpeed = speed;
}

void Vehicle::setMaxTurnSpeed(float speed)
{
	maxTurningSpeed = speed;
}

float Vehicle::getTurnRate()
{
	return vehTurningSpeed;
}