#ifndef COLLISION_H
#define COLLISION_H

#include "Vertex.h"
#include "Mesh.h"
#include "Entity.h"
#include "collisionDir.h"
#include <vector>

class Collision
{
private:

public:
	Collision();
	~Collision();

	static bool CheckCollision(Entity* one, Entity* two);
	collDir getDir(Vector3 dir);
	void update(float dt);
	
};

#endif