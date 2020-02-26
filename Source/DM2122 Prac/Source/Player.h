#ifndef PLAYER_H
#define PLAYER_H

#include "Vehicle.h"

class Player
{	
public:
	enum PLAYER_INPUT
	{
		UP = 0,
		DOWN,
		LEFT,
		RIGHT,
		ENTER,

		INPUT_TOTAL,
	};

	Player();
	~Player();

	int getKills();
	int getInput(PLAYER_INPUT key);
	Color getColor();
	std::string getName();
	Vehicle* getVehicle();

	void setKills(int killNum);
	void setInput(PLAYER_INPUT key, int newKey);
	void setColor(Color newColor);
	void setName(std::string string);
	void setVehicle(int vehicleSlot);

private:
	int kills;
	int inputs[INPUT_TOTAL];
	Color color;
	std::string name;
	Vehicle* vehicle;

};

#endif