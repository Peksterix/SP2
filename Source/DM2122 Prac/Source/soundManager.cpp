#include "soundManager.h"
#include "StateManager.h"

soundManager::soundManager()
{

}

soundManager::~soundManager()
{

}

void soundManager::play2DSound(string sound, bool isLoop, float volume)
{
    string preString = "audio/";
    string fullString;
    fullString = preString += sound += ".mp3";
    Engine = irrklang::createIrrKlangDevice();

    playSound = Engine->addSoundSourceFromFile(fullString.c_str());
    vol = volume / 10;
    playSound->setDefaultVolume(vol);

    Engine->play2D(playSound, isLoop);

    if (StateManager::getInstance()->getGameState() == StateManager::GAME_STATES::S_GAME)
    {
        Engine->stopAllSounds();
        Engine->drop();
    }

}

void soundManager::UpdateVol(float a)
{
    Engine->setSoundVolume(a / 10.0);
}

/*
void play3DSound(string sound, float posX, float posY, float posZ)
{
	sound += ".mp3";
	irrklang::ISoundEngine* Engine = irrklang::createIrrKlangDevice();
	Engine->play3D(sound.c_str());
}

float soundManager::getPosX()
{
	return posX;
}

void soundManager::setPosX(float X)
{
	posX = X;
}

float soundManager::getPosY()
{
	return posY;
}

void soundManager::setPosY(float Y)
{
	posY = Y;
}

float soundManager::getPosZ()
{
	return posZ;
}

void soundManager::setPosZ(float Z)
{
	posZ = Z;
}
*/