#include "Vehicle.h"

Vehicle::Vehicle()
{
	chassis = nullptr;
	wheel = nullptr;
	weapon = nullptr;
}

Vehicle::~Vehicle()
{
	if (chassis != nullptr) delete chassis;
	if (wheel != nullptr) delete wheel;
	if (weapon != nullptr) delete weapon;
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

void Vehicle::setChassis(int Type)
{
	if (chassis != nullptr) delete chassis;
	if (Type != -1)
	{
		chassis = new Chassis(Type);
		wheelPos.clear(), weaponPos.clear(), wheelScale.clear(), weaponScale.clear();
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
	if (Type != -1) wheel = new Wheel(Type);
	else wheel = nullptr;
}

void Vehicle::setWeapon(int Type)
{
	if (weapon != nullptr) delete weapon;
	if (Type != -1) weapon = new Weapon(Type);
	else weapon = nullptr;
}

void Vehicle::Update()
{
	if (chassis != nullptr)
	{
		if (chassis->getHealth() <= 0) delete chassis;
	}

	if (wheel != nullptr)
	{
		if (wheel->getHealth() <= 0) delete wheel;
	}

	if (weapon != nullptr)
	{
		if (weapon ->getHealth() <= 0) delete weapon;
	}

}