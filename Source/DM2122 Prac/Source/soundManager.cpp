#include "soundManager.h"

#include "OptionMenu.h"

soundManager* soundManager::instance = nullptr;

soundManager::soundManager()
{
    //Engine = irrklang::createIrrKlangDevice();

}

soundManager::~soundManager()
{
    //Engine->drop();
    for (int i = Engine.size(); i > 0; --i)
    {
        Engine[i]->drop();
        Engine.pop_back();
    }
}

void soundManager::play2DSound(string sound, bool isLoop)
{
    string preString = "audio/";
    string fullString;
    fullString = preString += sound += ".mp3";
    Engine.push_back(irrklang::createIrrKlangDevice());

    playSound = Engine[Engine.size() - 1]->addSoundSourceFromFile(fullString.c_str());
    Engine[Engine.size() - 1]->setSoundVolume(OptionMenu::getInstance()->getVolume() / 10.f);

    Engine[Engine.size() - 1]->play2D(playSound, isLoop);
}

void soundManager::UpdateVol(float a)
{
    //Engine->setSoundVolume(a / 10.f);   
    for (int i = Engine.size(); i > 0; --i)
    {
        Engine[i - 1]->setSoundVolume(a / 10.f);
    }
}

void soundManager::StopSounds()
{
    for (int i = Engine.size(); i > 0; --i)
    {
        Engine[i - 1]->removeAllSoundSources();
        Engine[i - 1]->stopAllSounds();
        Engine[i - 1]->drop();
        Engine.pop_back();
    }
    //Engine->drop();
}

soundManager* soundManager::getInstance()
{
    if (instance == nullptr)
        instance = new soundManager;
    return instance;
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