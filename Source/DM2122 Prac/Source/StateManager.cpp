#include "StateManager.h"

StateManager* StateManager::instance = nullptr;

StateManager::StateManager()
{
	currentState = GAME_STATES::S_MAINMENU;
}

StateManager::~StateManager()
{

}

StateManager* StateManager::getInstance()
{
	if (instance == nullptr)
		instance = new StateManager;
	return instance;
}

void StateManager::setGameState(GAME_STATES state)
{
	currentState = state;
}

StateManager::GAME_STATES StateManager::getGameState()
{
	return currentState;
}