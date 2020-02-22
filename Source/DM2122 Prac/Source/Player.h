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

	int getInput(PLAYER_INPUT key);
	void setInput(PLAYER_INPUT key, int newKey);
	Vehicle* getVehicle();
	void setVehicle(int vehicleSlot);

private:
	int inputs[INPUT_TOTAL];
	Vehicle* vehicle;

};

#endif