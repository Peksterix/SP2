#pragma once

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
		S_FREECAM, // Debug
		S_GAME,

		S_TOTAL,
	};

	StateManager();
	~StateManager();
	
	static StateManager* getInstance();
	void setGameState(GAME_STATES state);
	GAME_STATES getGameState();

private:

	static StateManager* instance;
	GAME_STATES currentState;

};