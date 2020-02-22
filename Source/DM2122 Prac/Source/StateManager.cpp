#include "StateManager.h"
#include "TestScene.h"
#include "VehicleScene.h"
#include "GameScene0.h"

StateManager* StateManager::instance = nullptr;

StateManager::StateManager()
{
	currentScene = nullptr;
	currentState = GAME_STATES::S_MAINMENU;
	currentSceneState = SCENE_STATES::SS_MAINMENU;
}

StateManager::~StateManager()
{
	delete currentScene;
}

void StateManager::setGameState(GAME_STATES state)
{
	currentState = state;
}

void StateManager::setScene(SCENE_STATES scene)
{
	if (currentScene != nullptr)
	{
		currentScene->Exit();
		delete currentScene;
	}
	
	if (scene == SCENE_STATES::SS_TEST) currentScene = new TestScene;
	else if (scene == SCENE_STATES::SS_MAINMENU) currentScene = new VehicleScene;
	else if (scene == SCENE_STATES::SS_MAP0) currentScene = new GameScene0;

	currentScene->Init();
	currentScene->Update(0.02);
}

StateManager* StateManager::getInstance()
{
	if (instance == nullptr)
		instance = new StateManager;
	return instance;
}

Scene* StateManager::getScene()
{
	return currentScene;
}

StateManager::SCENE_STATES StateManager::getSceneState()
{
	return currentSceneState;
}

StateManager::GAME_STATES StateManager::getGameState()
{
	return currentState;
}