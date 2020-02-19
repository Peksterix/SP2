#ifndef CAMERA_H
#define CAMERA_H

#include "Vertex.h"
#include "Entity.h"

class Camera : public Entity
{
public:
	Position target;
	Position up;
	double cameraAngleX, cameraAngleY;

	Camera();
	~Camera();
	void Init(const Position& pos, const Position& target, const Position& up);
	void Reset();
	void Update(double dt, Entity* obj, Position offset = 0, Position targetOffset = 0);
	void Update(double dt, double angleX = 0, double angleY = 0);
};

#endif