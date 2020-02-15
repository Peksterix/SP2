#ifndef OBJECT_H
#define OBJECT_H

#include "Vertex.h"
#include "Mesh.h"

class Object
{
private:

public:
	Position position;
	Position rotate;
	Position scale;

	Object();
	~Object();
};

#endif