#ifndef ANIMATE_H
#define ANIMATE_H

#include "Vertex.h"
#include <vector>

class Animate
{
private:
	std::vector<float*> bufferVal;
	std::vector<float> buffer[4];

public:

	Animate();
	~Animate();

	void AddToBuffer(float &Val, float Start, float End, float Duration, bool Stop);
	bool Update(double dt);
};

#endif