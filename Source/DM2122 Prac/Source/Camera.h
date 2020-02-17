#ifndef CAMERA_H
#define CAMERA_H

#include "Vertex.h"
#include "Entity.h"

class Camera : public Entity
{
public:
	Position target;
	Position up;
	int type;

	Camera();
	~Camera();
	void Init(const Position& pos, const Position& target, const Position& up, float type);
	void Reset();
	void Update(double dt, Entity* obj = nullptr, Position offset = 0, Position targetOffset = 0);
};

#endif