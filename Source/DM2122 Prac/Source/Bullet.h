#pragma once
#include "Entity.h"

class Bullet : public Entity
{
private:
	int playerID;

	float	xDir,
			zDir,
			speed,
			damage;

public:
	Bullet();
	Bullet(Position start, Position dir, float vel, float dam, int ID);
	~Bullet();

	void Update();
	
	float getDamage();
	int getID();
};