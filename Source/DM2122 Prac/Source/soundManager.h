#pragma once
#include "../../irrklang/include/irrKlang.h"
#pragma comment(lib, "../../irrklang/lib/irrKlang.lib")
#include <string>
#include <vector>

using namespace std;

class soundManager
{
private:
	//float posX;
	//float posY;
	//float posZ;
	
	static soundManager* instance;

	string addSound;

	float vol;
	//irrklang::ISoundEngine* Engine;
	std::vector<irrklang::ISoundEngine*> Engine;
	irrklang::ISoundSource* playSound;

public:
	soundManager();
	~soundManager();
	void play2DSound(string sound, bool isLoop);
	void UpdateVol(float);
	void StopSounds();

	static soundManager* getInstance();

	/*void play3DSound(string sound, float posX, float posY, float posZ);

	float getPosX();
	void setPosX(float X);
	float getPosY();
	void setPosY(float Y);
	float getPosZ();
	void setPosZ(float Z);*/
};