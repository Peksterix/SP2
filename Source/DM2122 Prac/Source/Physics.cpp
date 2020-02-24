#include "physics.h"

physics::physics()
{
	mass = 1.f;
	size = 1.f; // Tie to the Bounding Box scale, which is tied to the Mesh

	velocity = (0, 0, 0);
	acceleration = (0, 0, 0);

	up.Set(0, 1, 0);
	right.Set(1, 0, 0);
	front.Set(0, 0, -1);

	//values can be changed depending on what kind of physics you want(if that makes sense xd)
	frictionalForce = -0.3f;
	minVelo = 0.01f;
	KElost = 0.99f;
	gravity = -9.8;
	inertiaCoeff = 10.f;
}

physics::~physics()
{

}

float calcTforce(float a, float b)
{
	if (a > 0)
	{
		if (a + b < 0)
		{
			return 0;
		}

		return a + b;
	}

	else if (a < 0)
	{
		if (a + b > 0)
		{
			return 0;
		}

		return a + b;
	}

	return a + b;
}

void physics::Update()
{
	Vector3 traction;
	Vector3 tractionAccel;
	Vector3 friction;
	Vector3 frictionAccel;

	//drag force of vehicle
	traction.x = frictionalForce * (pow(velocity.x, 2)) / 2 * size;
	traction.y = frictionalForce * (pow(velocity.y, 2)) / 2 * size;
	traction.z = frictionalForce * (pow(velocity.z, 2)) / 2 * size;

	//decceleration caused by traction
	tractionAccel.x = traction.x / mass;
	tractionAccel.y = traction.y / mass;
	tractionAccel.z = traction.z / mass;

	tractionAccel.x = tractionAccel.x * (-velocity.x);
	tractionAccel.y = tractionAccel.y * (-velocity.y);
	tractionAccel.z = tractionAccel.z * (-velocity.z);

	acceleration += tractionAccel;

	//Friction
	friction.x = frictionalForce * (mass * gravity);
	friction.z = frictionalForce * (mass * gravity);

	frictionAccel.x = friction.x / mass;
	frictionAccel.z = friction.z / mass;
	frictionAccel.x = friction.x * (-velocity.x);
	frictionAccel.z = friction.z * (-velocity.z);
	acceleration += frictionAccel;
	velocity.y += acceleration.y;

	if (velocity.x > 0)
	{
		if (acceleration.x + velocity.x >= 0)
		{
			velocity.x += acceleration.x;
		}

		else
		{
			velocity.x = 0;
		}
	}

	else
	{
		velocity.x += acceleration.x;
	}

	if (velocity.z > 0)
	{
		if (acceleration.z + velocity.z >= 0)
		{
			velocity.z += acceleration.z;
		}

		else
		{
			velocity.z = 0;
		}
	}

	else
	{
		velocity.z += acceleration.z;
	}

	preCollVelo = velocity;

	//TODO: add force decay
	if (force.x > 0)
	{
		force.x = calcTforce(force.x, -getMass() / inertiaCoeff);
	}
	else if (force.x < 0)
	{
		force.x = calcTforce(force.x, getMass() / inertiaCoeff);
	}

	if (force.y > 0)
	{
		force.y = calcTforce(force.y, -getMass() / inertiaCoeff);
	}
	else if (force.y < 0)
	{
		force.y = calcTforce(force.y, getMass() / inertiaCoeff);
	}

	if (force.z > 0)
	{
		force.z = calcTforce(force.z, -getMass() / inertiaCoeff);
	}
	else if (force.z < 0)
	{
		force.z = calcTforce(force.z, getMass() / inertiaCoeff);
	}
}
/*
void physics::rigidBody(physics* collidingRB, Vector3 penetrationDepth, Vector3 currentPos, Vector3 collidingPos, Directions dir)
{
	switch (dir)
	{
	case POSX:
	case NEGX:
		if (abs(velocity.x) > minVelo)
		{
			velocity.x = -velocity.x * KElost;
		}

		else
		{
			velocity.x = 0;
		}

		break;
	case POSZ:
	case NEGZ:
		if (abs(velocity.z) > minVelo)
		{
			velocity.z = -velocity.z * KElost;
		}

		else
		{
			velocity.z = 0;
		}

		break;
	default:
		break;
	}
}
*/

void physics::setVelo(Vector3 v)
{
	velocity = v;
}

Vector3 physics::getVelo()
{
	return velocity;
}

void physics::setPreCollVelo(Vector3 preV)
{
	preCollVelo = preV;
}

Vector3 physics::getPreCollVelo()
{
	return preCollVelo;
}

void physics::setAccel(Vector3 accel)
{
	acceleration = accel;
}

Vector3 physics::getAccel()
{
	return acceleration;
}

void physics::setForce(Vector3 force)
{
	this->force = force;
}

Vector3 physics::getForce()
{
	return force;
}

void physics::setUp(Vector3 up)
{
	this->up = up;
}

void physics::setRight(Vector3 right)
{
	this->right = right;
}

void physics::setFront(Vector3 front)
{
	this->front = front;
}

Vector3 physics::getUp()
{
	return up;
}

Vector3 physics::getRight()
{
	return right;
}

Vector3 physics::getFront()
{
	return front;
}

void physics::setMass(float mass)
{
	this->mass = mass;
}

float physics::getMass()
{
	return mass;
}

void physics::setSize(float size)
{
	this->size = size;
}

float physics::getSize()
{
	return size;
}

void physics::addForce(Vector3 newF)
{
	force += newF;
}