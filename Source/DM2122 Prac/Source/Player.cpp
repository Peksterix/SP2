#include "Player.h"
#include "Application.h"

Player::Player()
{
	inputs[UP] = 'W';
	inputs[DOWN] = 'S';
	inputs[LEFT] = 'A';
	inputs[RIGHT] = 'D';
	inputs[ENTER] = VK_RETURN;

	name = "Player";
	vehicle = nullptr;
}

Player::~Player()
{
	if (vehicle != nullptr) delete vehicle;
}

int Player::getInput(PLAYER_INPUT key)
{
	return inputs[key];
}

std::string Player::getName()
{
	return name;
}

Vehicle* Player::getVehicle()
{
	return vehicle;
}

void Player::setInput(PLAYER_INPUT key, int newKey)
{
	inputs[key] = newKey;
}

void Player::setName(std::string string)
{
	name = string;
}

void Player::setVehicle(int vehicleSlot)
{
	if (vehicle != nullptr) delete vehicle;
	vehicle = new Vehicle;

	// Access vehicle in vehcile slot

	// Temp
	vehicle->setChassis(vehicleSlot);
	vehicle->setWheel(vehicleSlot);
	vehicle->setWeapon(vehicleSlot);
}