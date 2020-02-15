#ifndef CAMERA_H
#define CAMERA_H

#include "Vertex.h"
//#include "Vector3.h"
#include "Object.h"

class Camera : public Object
{
public:
	Position target;
	Position up;
	int type;

	Camera();
	~Camera();
	void Init(const Position& pos, const Position& target, const Position& up, float& type);
	void Reset();
	void Update(double dt, Object* obj = nullptr, Position offset = 0, Position targetOffset = 0);
};

#endif