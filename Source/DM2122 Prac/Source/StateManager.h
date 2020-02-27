#pragma once
#include "Scene.h"

class StateManager
{
public:
	enum class GAME_STATES
	{
		S_LOADOUT_PLAY = 0,
		S_LOADOUT_CUSTOM,
		S_CUSTOMISE,
		S_MAINMENU,
		S_OPTIONS,
		S_EDITNAMES,
		S_GAME,
		S_GAMEOVER,
		S_FREECAM, // Debug

		S_TOTAL,
	};

	enum class SCENE_STATES
	{
		SS_TEST = 0,
		SS_LOADING,
		SS_MAINMENU,
		SS_MAP0,

		SS_TOTAL,
	};

	StateManager();
	~StateManager();
	
	static StateManager* getInstance();
	
	void setGameState(GAME_STATES state);
	GAME_STATES getGameState();
	
	void setScene(SCENE_STATES scene);
	SCENE_STATES getSceneState();
	Scene* getScene();

private:

	static StateManager* instance;
	GAME_STATES currentState;
	SCENE_STATES currentSceneState;
	Scene* currentScene;

};