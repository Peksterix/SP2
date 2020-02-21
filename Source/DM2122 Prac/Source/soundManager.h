#pragma once
#include "../../irrklang/include/irrKlang.h"
#pragma comment(lib, "../../irrklang/lib/irrKlang.lib")
#include <string>

using namespace std;

class soundManager
{
private:
	float posX;
	float posY;
	float posZ;
	string addSound;

public:
	soundManager();
	~soundManager();
	void play2DSound(string sound);
	/*void play3DSound(string sound, float posX, float posY, float posZ);

	float getPosX();
	void setPosX(float X);
	float getPosY();
	void setPosY(float Y);
	float getPosZ();
	void setPosZ(float Z);*/
};