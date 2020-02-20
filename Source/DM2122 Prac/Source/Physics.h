#pragma once
//#include "directions.h"
#include "Source/TestScene.h"

class physics
{

private:
	float mass;
	float size;
	Vector3 velocity;
	Vector3 preCollVelo;
	Vector3 acceleration;
	Vector3 up;
	Vector3 right;
	Vector3 front;
	Vector3 force;

public:

	float frictionalForce, minVelo, KElost, gravity;

	physics();
	~physics();

	void setMass();
	void getMass();

	void Update();
	void rigidBody(physics* collidingRB, Vector3 penetrationDepth, Vector3 currentPos, Vector3 collidingPos, Directions dir);

	Vector3 getUp();
	Vector3 getRight();
	Vector3 getFront();
	void setUp(Vector3 up);
	void setRight(Vector3 right);
	void setFront(Vector3 front);

	void setVelo(Vector3 v);
	Vector3 getVelo();
	void setPreCollVelo(Vector3 preV);
	Vector3 getPreCollVelo();
	void setAccel(Vector3 accel);
	Vector3 getAccel();
	void setForce(Vector3 force);
	Vector3 getForce();

	void Update();
};