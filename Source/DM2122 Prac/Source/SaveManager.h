#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include "Entity.h"
#include "Chassis.h"
#include "Weapon.h"
#include "Wheel.h"
#include <string>

class SaveManager : public Entity

{
private:
	
	//enum SLOT_MANAGER
	//{
	//	Chassis,
	//	Wheel,
	//	Weapon,
	//	Name,

	//};

public:

	SaveManager(); //constructor
	~SaveManager(); // destructor

	static void SaveVehicle(int num, int cha, int whe, int wea, std::string name); // save the data of vehicles
	static void SavePlayer(std::string name,int num); // save the data of players
	static void AccessVehicle(int num, int& cha, int& whe, int& wea, std::string& name); // access the data of vehicles

};

#endif
