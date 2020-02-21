#include "soundManager.h"

soundManager::soundManager()
{

}

soundManager::~soundManager()
{

}


void soundManager::play2DSound(string sound)
{
	sound += ".mp3";
	irrklang::ISoundEngine* Engine = irrklang::createIrrKlangDevice();
	Engine->play2D(sound.c_str());
}

//void play3DSound(string sound, float posX, float posY, float posZ)
//{
//	sound += ".mp3";
//	irrklang::ISoundEngine* Engine = irrklang::createIrrKlangDevice();
//	Engine->play3D(sound.c_str());
//}
//
//float soundManager::getPosX()
//{
//	return posX;
//}
//
//void soundManager::setPosX(float X)
//{
//	posX = X;
//}
//
//float soundManager::getPosY()
//{
//	return posY;
//}
//
//void soundManager::setPosY(float Y)
//{
//	posY = Y;
//}
//
//float soundManager::getPosZ()
//{
//	return posZ;
//}
//
//void soundManager::setPosZ(float Z)
//{
//	posZ = Z;
//}