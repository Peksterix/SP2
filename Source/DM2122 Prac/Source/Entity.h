#ifndef ENTITY_H
#define ENTITY_H

#include "Vertex.h"
#include "Physics.h"

class Entity
{
private:

public:
	Position position;
	Position rotate;
	Position scale;
	

	Entity();
	~Entity();
};

#endif