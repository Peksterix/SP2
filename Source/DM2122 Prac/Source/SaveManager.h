#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <string>

class SaveManager

{
private:

public:

	SaveManager(); //constructor
	~SaveManager(); // destructor

	static void SaveVehicle(int num, int cha, int whe, int wea, std::string name); // save the data of vehicles
	static void SavePlayer(std::string name, int num); // save the data of players
	static void AccessVehicle(int num, int& cha, int& whe, int& wea, std::string& name); // access the data of vehicles
	static void AccessPlayer(int num, std::string& name);

};

#endif