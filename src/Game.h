
#ifndef _GAME_H
#define _GAME_H

#include "main.h"

// The "Game" class handles all game functions
class Game
{
protected:
	GMapLoader* maploader;			// game's map loader.  takes GLevel objects as input
	Client* client;					// game's player

	GScenario* scenario;			// current scenario
	GLevel* level;					// current level.  must be in scenerio's level list

	SDL_Surface* background;		// current background the game is blitting. dependant on gscreen and gstate
	vector<CButton*> buttonList;	// list of buttons used by the game

	int gscreen;		// what screen the game is at (SCREEN_MENU, SCREEN_INGAME)
	int gstate;			// what state the game is at (MENU_HELPFILE, INGAME_PAUSED)
	int hmenu;			// help file menu index (starting from 0 to GAME_HELPFILE_PAGES-1)
	int timeLeft;		// current second counting down to 0
	int currTime, prevTime;		// used to determine time

	void setScreen(int _screen);
	void setScreen(int _screen, int _state);
	void setState(int _state);
	void EndGame(int cond);		// called when game ends according to parameter
	void LoseLevel(int cond);

	friend class Client;

private:
	// Screen/State changes
	void EnterTitleScreen();
	void EnterScenarioSel();
	void EnterLevelSel();
	void EnterHelpFile();
	void EnterGame();
	void GameContPrep();
	void ChangeHelpFilePage(int inc);
	void ChangeScenario(int inc);
	void ChangeLevel(int inc);
	void NextLevel();

	void WinLevel();
	void QuitLevel();
	void FinishLevel();
	void GamePause();
	void StartGame();			// called when player clicks "Start"

	void ResetGame();			// resets variables and reloads map
	void ClearGame();
	void LoadGame();

	map<string, SDL_Surface*>	dynSurfaces;	// dynamic surfaces blitted on the screen
	void SetDynSurface(string name, SDL_Surface* newsurface);

public:
	Game();
	virtual ~Game();
	void Initialize();
	void LoadButtons();

	void setClient(Client* c) { client = c; }
	Client* getClient() { return client; }
	bool inScreen(int _screen) { return gscreen == _screen; }
	int inState(int _state) { return gstate == _state; }
	bool isPaused();

	void LifeCycle();
	void HandleEvents();
	void HandleDynamicEvents();
	void ButtonAction(CButton* button);

	void Draw();
	void DrawButtons();
	void DrawDynSurfaces();

	void Update();
	void UpdateDynSurfaces();
	void UpdateBackground();
};

#endif
