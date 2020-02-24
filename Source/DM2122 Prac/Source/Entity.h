#ifndef ENTITY_H
#define ENTITY_H

#include "Vertex.h"
#include "Mesh.h"

class Entity
{
private:

public:
	Position position;
	Position rotate;
	Position scale;

	Mesh* mesh;

	Entity();
	~Entity();
};

#endif