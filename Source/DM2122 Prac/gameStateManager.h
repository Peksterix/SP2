#pragma once

class gameStateManager
{
private:
	enum gameStates
	{
		MAIN_MENU,
		VEHICLE_SELECTION,
		GAME,
		PAUSE,
		QUIT
	};

	static gameStateManager* instance;
	gameStates currentState;

public:
	gameStateManager();
	~gameStateManager();
	static gameStateManager* getInstance();
	void changeState(gameStates scene);
	gameStates getGameState();
};