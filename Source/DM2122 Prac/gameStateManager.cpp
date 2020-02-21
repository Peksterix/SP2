#include "gameStateManager.h"

gameStateManager* gameStateManager::instance;

gameStateManager::gameStateManager()
{

}

gameStateManager::~gameStateManager()
{

}

gameStateManager* gameStateManager::getInstance()
{
	if (instance == nullptr)
		instance = new gameStateManager;
	return instance;
}

void gameStateManager::changeState(gameStates state)
{
	currentState = state;
}

gameStateManager::gameStates gameStateManager::getGameState()
{
	return currentState;
}