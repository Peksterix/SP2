#ifndef COLLISION_H
#define COLLISION_H

#include "Vertex.h"
#include "Mesh.h"
#include "Entity.h"
#include <vector>

class Collision
{
private:

public:
	Collision();
	~Collision();

	bool CheckCollision(Entity* one, Entity* two);

};

#endif