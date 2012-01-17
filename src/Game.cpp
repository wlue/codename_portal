
#include "Game.h"
#include "GMapLoader.h"
#include "CObject.h"
#include "CPlayer.h"
#include "CButton.h"
#include "GScenario.h"
#include "GLevel.h"

Game::Game()
{
	
}

Game::~Game()
{

}

void Game::Initialize()
{
	// Initialize game variables
	client = NULL;
	maploader = new GMapLoader();

	scenario = NULL;
	level = NULL;
	gscreen = SCREEN_MENU;
	gstate = MENU_TITLE;
	hmenu = 0;
	timeLeft = prevTime = currTime = 0;

	LoadButtons();
	UpdateBackground();
}

void Game::setScreen(int _screen)
{
	gscreen = _screen;
	setState(0);
}

void Game::setScreen(int _screen, int _state)
{
	gscreen = _screen;
	setState(_state);
}

void Game::setState(int _state)
{
	gstate = _state;
	Update();
}

void Game::LifeCycle()
{
	if(!client) return;
	
	if(gscreen == SCREEN_GAME && gstate < GAME_PAUSED)
	{
		// When game is in game screen, allow all objects on the map to move/interact freely
		for(int i=0; i<int(listobjs.size()); ++i)
			listobjs.at(i)->LifeCycle();

		// Update time
		prevTime = currTime;
		currTime = (int) time(NULL); 
		if(currTime - prevTime >= 1) 
		{
			// When one second has passed, decrease the timer
			timeLeft = maxVal(0, timeLeft - 1);
			client->UpdateHUD();	// Update the client's HUD
			if(timeLeft <= 0)
				LoseLevel(GAMELOSE_TIME);	// Time's up!  Player has lost
		}

	}
}

void Game::HandleDynamicEvents()
{
	if(!client) return;
	if(gscreen == SCREEN_MENU || gstate >= GAME_PAUSED)
		client->HandleDynamicEvents();
	else
		client->HandlePlayerDynEvents();
}

void Game::HandleEvents()
{
	if(!client) return;

	if(event.type == SDL_KEYDOWN && event.key.keysym.sym == PLAYER_KEY_PAUSE)
		game.GamePause();

	if(gscreen == SCREEN_MENU || gstate >= GAME_PAUSED)
		client->HandleEvents();
	else
		client->HandlePlayerEvents();
}


// Screen/State changes

void Game::EnterTitleScreen()
{
	setScreen(SCREEN_MENU);
}

void Game::EnterScenarioSel()
{
	// default scenario is the first one in the list
	scenario = *listscn.begin();
	setState(MENU_SCENESEL);
}

void Game::EnterLevelSel()
{
	if(!scenario) return;
	// default level is the first one in the list
	level = *scenario->getLevels().begin();
	setState(MENU_LEVELSEL);
}

void Game::EnterHelpFile()
{
	hmenu = 0;
	setState(MENU_HELPFILE);
}

void Game::EnterGame()
{
	// Do not allow them to start the game if a level or scenario hasn't been picked yet
	if(!level || !scenario) return;
	StartGame();
}

void Game::ChangeHelpFilePage(int inc)
{
	// changing help file page
	hmenu += inc;
	while(hmenu < 0) hmenu += GAME_HELPFILE_PAGES;
	while(hmenu >= GAME_HELPFILE_PAGES) hmenu -= GAME_HELPFILE_PAGES;
	Update();
}

void Game::ChangeScenario(int inc)
{
	// changing scenario
	scenario = nextItem(listscn, scenario, inc);
	Update();
}

void Game::ChangeLevel(int inc)
{
	// changling level
	level = scenario->nextLevel(level, inc);
	Update();
}

void Game::WinLevel()
{
	FinishLevel();
}

void Game::LoseLevel(int cond)
{
	setState(GAME_LOSE);
}

void Game::QuitLevel()
{
	// Game is cleared, and enters the title screen when the game is quit
	ClearGame();
	EnterTitleScreen();
}

void Game::FinishLevel()
{
	// If the next level repeats back to the first level, they have finished the scenario
	if(scenario->nextLevel(level, 1) == *scenario->getLevels().begin())
		setState(GAME_FINISHSCN);
	else
		setState(GAME_FINISHLVL);
}

// Game Screen Functions

void Game::StartGame()
{
	if(!scenario || !level) return;
	LoadGame();
	setScreen(SCREEN_GAME, GAME_PREP);
}

void Game::EndGame(int cond)
{
	// Call different methods depending on the condition of the endgame
	switch(cond)
	{
	case GAMEEND_WIN:
		WinLevel();		// Player has won
		break;
	case GAMEEND_RETRY:
		ResetGame();	// Player wants to retry the level
		break;
	case GAMEEND_QUIT:
		QuitLevel();	// Player gives up or finishes scenario, and quits to menu screen
		break;
	}
}

void Game::GameContPrep()
{
	setState(GAME_PLAYING);
}

void Game::GamePause()
{
	// Pause when game is already paused, and unpause when game is paused
	if(gstate == GAME_PLAYING)		setState(GAME_PAUSED);
	else if(gstate == GAME_PAUSED)	setState(GAME_PLAYING);
}

void Game::ResetGame()
{
	// Clear, and start the game
	ClearGame();
	StartGame();
}

void Game::ClearGame()
{
	// Delete all of the objects in the map except the client's player
	deletion = listobjs;
	RemoveElement<CObject*>(deletion, client->getControl());

	if(!deletion.size()) return;
	vector<CObject*>::iterator f;
	for(f=deletion.begin(); f!=deletion.end(); ++f)
		delete *f;
	deletion.clear();

}

void Game::LoadGame()
{
	maploader->LoadLevel(level);		// Load the map given the level
	timeLeft = level->getTimeLimit();	// Set up the timer
	client->SetupPlayer();				// Set up the player

}

void Game::NextLevel()
{
	// Set the game's level to its next in sequential order, then reset the game
	GLevel* nextlevel = scenario->nextLevel(level, 1);
	level = nextlevel;
	ResetGame();
}

// Drawing

void Game::SetDynSurface(string name, SDL_Surface* newsurface)
{
	// Frees and resets a dynamically generated surface. 
	// DO NOT USE WITH SURFACE LOADED WITH ImageHandler
	SDL_Surface* surface = dynSurfaces[name];
	SDL_FreeSurface(surface);
	dynSurfaces[name] = newsurface;
}

void Game::Update()
{
	// Update background and dynamic surface text when game states change
	UpdateBackground();
	UpdateDynSurfaces();
}

void Game::UpdateDynSurfaces()
{
	switch(gscreen)
	{
	case SCREEN_MENU:
		switch(gstate)
		{
		case MENU_SCENESEL:
		{
			string num, name, diff, lvls;
			if(scenario != NULL)
			{
				num = "Scenario " + val2str(itemIndex(listscn, scenario) + 1) + " of " + val2str(listscn.size());
				name = scenario->getName();
				diff = "Difficulty: " + scenario->getDifficulty();
				lvls = "Levels: " + val2str(scenario->getLevelSize());
			}
			else
			{
				num = "Scenario 0 of " + val2str(listscn.size());
				name = "No Scenario Selected";
				diff = "Difficulty: ?";
				lvls = "Levels: ? ";
			}

			dynSurfaces["menu_scn_Prev"] = scenario->getPrevImg();
			SetDynSurface("menu_scn_Num", TTF_DrawText_Blended("text", num, "black", 0));
			SetDynSurface("menu_scn_Name", TTF_DrawText_Blended("btitle", name, "black", 0));
			SetDynSurface("menu_scn_Diff", TTF_DrawText_Blended("text", diff, "black", 0));
			SetDynSurface("menu_scn_Lvls", TTF_DrawText_Blended("text", lvls, "black", 0));
			break;
		}
		case MENU_LEVELSEL:
		{
			string scn, name, diff, time, num;
			scn = "Scenario: " + scenario->getName();
			diff = "Difficulty:";
			if(level != NULL)
			{
				name = level->getName();
				time = "Time Limit: " + sec2clock(level->getTimeLimit());
				num = "Level " + val2str(itemIndex(scenario->getLevels(), level) + 1) + " of " + val2str(scenario->getLevelSize());
			}
			else
			{
				name = "No Level Selected";
				time = "Time Limit: ?";
				num = "Level 0 of " + val2str(scenario->getLevelSize());
			}

			SetDynSurface("menu_lvl_Scn", TTF_DrawText_Blended("text", scn, "black", 0));
			SetDynSurface("menu_lvl_Name", TTF_DrawText_Blended("btitle", name, "black", 0));
			SetDynSurface("menu_lvl_Diff", TTF_DrawText_Blended("text", diff, "black", 0));
			SetDynSurface("menu_lvl_Time", TTF_DrawText_Blended("text", time, "black", 0));
			SetDynSurface("menu_lvl_Lvl", TTF_DrawText_Blended("text", num, "black", 0));
			dynSurfaces["menu_lvl_DiffStar"] = imgHandler->getImage("panel")->getImage("star" + val2str(level->getDifficulty()), 0, false);
		}
		}
		break;
	case SCREEN_GAME:
		switch(gstate)
		{
		case GAME_PAUSED:
			dynSurfaces["game_pause_Panel"] = imgHandler->getImage("panel")->getImage("pause", 0, false);
			break;
		case GAME_PREP:
		{
			string scn, lvl;
			scn = scenario->getName();
			lvl = level->getName();
			
			dynSurfaces["game_prep_Panel"] = imgHandler->getImage("panel")->getImage("prep", 0, false);
			SetDynSurface("game_prep_Scn", TTF_DrawText_Blended("text", scn, "black", 0));
			SetDynSurface("game_prep_Lvl", TTF_DrawText_Blended("text", lvl, "black", 0));
			break;
		}
		case GAME_FINISHLVL:
		{
			string time;
			time = "Time Elapsed: " + sec2clock(level->getTimeLimit() - timeLeft);

			dynSurfaces["game_flvl_Panel"] = imgHandler->getImage("panel")->getImage("flevel", 0, false);
			SetDynSurface("game_flvl_Time", TTF_DrawText_Blended("text", time, "black", 0));
			break;
		}
		case GAME_FINISHSCN:
		{
			string time;
			time = "Time Elapsed: " + sec2clock(level->getTimeLimit() - timeLeft);

			dynSurfaces["game_fscn_Panel"] = imgHandler->getImage("panel")->getImage("fscenario", 0, false);
			SetDynSurface("game_fscn_Time", TTF_DrawText_Blended("text", time, "black", 0));
			break;
		}
		case GAME_LOSE:
		{
			string text;
			if(client->getControl()->getHealth() <= 0)
				text = "You have died";
			else if(timeLeft <= 0)
				text = "Time's up";
			else
				text = "You have lost";

			dynSurfaces["game_lose_Panel"] = imgHandler->getImage("panel")->getImage("generic", 0, false);
			SetDynSurface("game_lose_Text", TTF_DrawText_Blended("text", text, "black", 0));
			break;
		}
		}
		break;
	}
}

void Game::UpdateBackground()
{
	string imgname;
	switch(gscreen)
	{
	case SCREEN_MENU:
		switch(gstate)
		{
		case MENU_TITLE:
			imgname = "title";
			break;
		case MENU_HELPFILE:
			imgname = "helpfile_p" + val2str(hmenu);
			break;
		case MENU_SCENESEL:
			imgname = "scenario";
			break;
		case MENU_LEVELSEL:
			imgname = "level";
			break;
		}
		break;
	case SCREEN_GAME:
		imgname = "game";
		break;
	}
		
	background = imgHandler->getImage("background")->getImage(imgname, 0, false);
}

void Game::Draw()
{
	if(!client) return;

	SDL_BlitSurface(background, NULL, screen, NULL);

	if(gscreen == SCREEN_GAME)
	{
		client->draw();
		if(gstate >= GAME_PAUSED)
			boxRGBA(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0, 128);
	}

	DrawDynSurfaces();
	DrawButtons();
}

void Game::DrawButtons()
{
	vector<CButton*>::iterator b;
	for(b=buttonList.begin(); b!=buttonList.end(); ++b)
		if((*b)->inScreen(gscreen, gstate))
			(*b)->draw();
}

void Game::DrawDynSurfaces()
{
	switch(gscreen)
	{
	case SCREEN_MENU:
		switch(gstate)
		{
		case MENU_SCENESEL:
			ApplySurface(dynSurfaces["menu_scn_Num"], 
				SCREEN_WIDTH/2 - dynSurfaces["menu_scn_Num"]->w/2, MENU_MARGIN_TOP);
			ApplySurface(dynSurfaces["menu_scn_Name"], 
				MENU_SCENE_IMG_LEFT + MENU_SCENE_IMG_WIDTH + MENU_MARGIN_LEFT, MENU_SCENE_IMG_TOP);
			ApplySurface(dynSurfaces["menu_scn_Diff"], 
				MENU_SCENE_IMG_LEFT + MENU_SCENE_IMG_WIDTH + MENU_MARGIN_LEFT, MENU_SCENE_IMG_TOP + 70);
			ApplySurface(dynSurfaces["menu_scn_Lvls"], 
				MENU_SCENE_IMG_LEFT + MENU_SCENE_IMG_WIDTH + MENU_MARGIN_LEFT, MENU_SCENE_IMG_TOP + 70 + 45);
			ApplySurface(dynSurfaces["menu_scn_Prev"], 
				MENU_SCENE_IMG_LEFT, MENU_SCENE_IMG_TOP);
			break;
		case MENU_LEVELSEL:
			ApplySurface(dynSurfaces["menu_lvl_Scn"], 
				MENU_MARGIN_LEFT, MENU_MARGIN_TOP);
			ApplySurface(dynSurfaces["menu_lvl_Name"], 
				MENU_MARGIN_LEFT, MENU_MARGIN_TOP + 80);
			ApplySurface(dynSurfaces["menu_lvl_Diff"], 
				MENU_MARGIN_LEFT, MENU_MARGIN_TOP + 80 + 70);
			ApplySurface(dynSurfaces["menu_lvl_DiffStar"], 
				MENU_MARGIN_LEFT + 160, MENU_MARGIN_TOP + 80 + 70);
			ApplySurface(dynSurfaces["menu_lvl_Time"], 
				MENU_MARGIN_LEFT, MENU_MARGIN_TOP + 80 + 70 + 45);
			ApplySurface(dynSurfaces["menu_lvl_Lvl"], 
				SCREEN_WIDTH/2 - dynSurfaces["menu_lvl_Lvl"]->w/2, SCREEN_HEIGHT - 160);
			break;
		}
		break;
	case SCREEN_GAME:
		switch(gstate)
		{
		case GAME_PAUSED:
			ApplySurface(dynSurfaces["game_pause_Panel"], 
				SCREEN_WIDTH/2 - dynSurfaces["game_pause_Panel"]->w/2, SCREEN_HEIGHT/2 - dynSurfaces["game_pause_Panel"]->h/2);
			break;
		case GAME_PREP:
			ApplySurface(dynSurfaces["game_prep_Panel"], 
				SCREEN_WIDTH/2 - dynSurfaces["game_prep_Panel"]->w/2, SCREEN_HEIGHT/2 - dynSurfaces["game_prep_Panel"]->h/2);
			ApplySurface(dynSurfaces["game_prep_Scn"], 
				SCREEN_WIDTH/2 - dynSurfaces["game_prep_Scn"]->w/2, SCREEN_HEIGHT/2 - dynSurfaces["game_prep_Panel"]->h/2 + 135);
			ApplySurface(dynSurfaces["game_prep_Lvl"], 
				SCREEN_WIDTH/2 - dynSurfaces["game_prep_Lvl"]->w/2, SCREEN_HEIGHT/2 - dynSurfaces["game_prep_Panel"]->h/2 + 135 + 80);
			break;
		case GAME_FINISHLVL:
			ApplySurface(dynSurfaces["game_flvl_Panel"], 
				SCREEN_WIDTH/2 - dynSurfaces["game_flvl_Panel"]->w/2, SCREEN_HEIGHT/2 - dynSurfaces["game_flvl_Panel"]->h/2);
			ApplySurface(dynSurfaces["game_flvl_Time"], 
				SCREEN_WIDTH/2 - dynSurfaces["game_flvl_Time"]->w/2, SCREEN_HEIGHT/2 - dynSurfaces["game_flvl_Panel"]->h/2 + 130);
			break;
		case GAME_FINISHSCN:
			ApplySurface(dynSurfaces["game_fscn_Panel"], 
				SCREEN_WIDTH/2 - dynSurfaces["game_fscn_Panel"]->w/2, SCREEN_HEIGHT/2 - dynSurfaces["game_fscn_Panel"]->h/2);
			ApplySurface(dynSurfaces["game_fscn_Time"], 
				SCREEN_WIDTH/2 - dynSurfaces["game_fscn_Time"]->w/2, SCREEN_HEIGHT/2 - dynSurfaces["game_fscn_Panel"]->h/2 + 130);
			break;		
		case GAME_LOSE:
			ApplySurface(dynSurfaces["game_lose_Panel"], 
				SCREEN_WIDTH/2 - dynSurfaces["game_lose_Panel"]->w/2, SCREEN_HEIGHT/2 - dynSurfaces["game_lose_Panel"]->h/2);
			ApplySurface(dynSurfaces["game_lose_Text"], 
				SCREEN_WIDTH/2 - dynSurfaces["game_lose_Text"]->w/2, SCREEN_HEIGHT/2 - dynSurfaces["game_lose_Panel"]->h/2 + 130);
			break;		
		}
	}
}

// Buttons

void Game::LoadButtons()
{
	// TITLE SCREEN
	buttonList.push_back(new CButton("title_startgame", 
									 SCREEN_MENU, MENU_TITLE, 
									 BUTTON_CENTER, MENU_TITLE_TOP));
	buttonList.push_back(new CButton("title_helpfile", 
									 SCREEN_MENU, MENU_TITLE, 
									 BUTTON_CENTER, MENU_TITLE_TOP + MENU_TITLE_INTERVAL));
	buttonList.push_back(new CButton("title_quit", 
									 SCREEN_MENU, MENU_TITLE, 
									 BUTTON_CENTER, MENU_TITLE_TOP + 2*MENU_TITLE_INTERVAL));

	// HELP FILE SCREEN
	buttonList.push_back(new CButton("menu_back", 
									 SCREEN_MENU, MENU_HELPFILE, 
									 BUTTON_RIGHT, BUTTON_BOTTOM));
	buttonList.push_back(new CButton("arrow_left", 
									 SCREEN_MENU, MENU_HELPFILE, 
									 SCREEN_WIDTH/2 - 100 - 86, BUTTON_BOTTOM));
	buttonList.push_back(new CButton("arrow_right", 
									 SCREEN_MENU, MENU_HELPFILE, 
									 SCREEN_WIDTH/2 + 100, BUTTON_BOTTOM));

	// SCENARIO SELECT SCREEN
	buttonList.push_back(new CButton("menu_enter", 
									 SCREEN_MENU, MENU_SCENESEL, 
									 SCREEN_WIDTH - 2*180 - 10, BUTTON_BOTTOM));
	buttonList.push_back(new CButton("menu_back", 
									 SCREEN_MENU, MENU_SCENESEL, 
									 BUTTON_RIGHT, BUTTON_BOTTOM));
	buttonList.push_back(new CButton("arrow_up", 
									 SCREEN_MENU, MENU_SCENESEL, 
									 BUTTON_LEFT, MENU_SCENE_IMG_TOP));
	buttonList.push_back(new CButton("arrow_down", 
									 SCREEN_MENU, MENU_SCENESEL, 
									 BUTTON_LEFT, MENU_SCENE_IMG_TOP + MENU_SCENE_IMG_HEIGHT - 86));

	// LEVEL SELECT SCREEN
	buttonList.push_back(new CButton("menu_start", 
									 SCREEN_MENU, MENU_LEVELSEL, 
									 BUTTON_LEFT, BUTTON_BOTTOM));
	buttonList.push_back(new CButton("menu_back", 
									 SCREEN_MENU, MENU_LEVELSEL, 
									 BUTTON_RIGHT, BUTTON_BOTTOM));
	buttonList.push_back(new CButton("arrow_left", 
									 SCREEN_MENU, MENU_LEVELSEL, 
									 BUTTON_LEFT, SCREEN_HEIGHT - 180));
	buttonList.push_back(new CButton("arrow_right", 
									 SCREEN_MENU, MENU_LEVELSEL, 
									 BUTTON_RIGHT, SCREEN_HEIGHT - 180));

	// GAME PREP SCREEN
	buttonList.push_back(new CButton("menu_start", 
									 SCREEN_GAME, GAME_PREP, 
									 BUTTON_CENTER, SCREEN_HEIGHT/2 + GPANEL_STANDARD_HEIGHT/2 - 130));
	// GAME PAUSE SCREEN
	buttonList.push_back(new CButton("menu_retry", 
									 SCREEN_GAME, GAME_PAUSED, 
									 BUTTON_CENTER, SCREEN_HEIGHT/2 - GPANEL_PAUSE_HEIGHT/2 + 150));
	buttonList.push_back(new CButton("menu_end", 
									 SCREEN_GAME, GAME_PAUSED, 
									 BUTTON_CENTER, SCREEN_HEIGHT/2 - GPANEL_PAUSE_HEIGHT/2 + 150 + 80));
	buttonList.push_back(new CButton("menu_quit", 
									 SCREEN_GAME, GAME_PAUSED, 
									 BUTTON_CENTER, SCREEN_HEIGHT/2 - GPANEL_PAUSE_HEIGHT/2 + 150 + 80 + 80));
	// GAME FINISH LEVEL SCREEN
	buttonList.push_back(new CButton("menu_nextlevel", 
									 SCREEN_GAME, GAME_FINISHLVL, 
									 BUTTON_CENTER, SCREEN_HEIGHT/2 + GPANEL_STANDARD_HEIGHT/2 - 130));
	// GAME FINISH SCENARIO SCREEN
	buttonList.push_back(new CButton("menu_mainmenu", 
									 SCREEN_GAME, GAME_FINISHSCN, 
									 BUTTON_CENTER, SCREEN_HEIGHT/2 + GPANEL_STANDARD_HEIGHT/2 - 130));
	// GAME LOSE SCREEN
	buttonList.push_back(new CButton("menu_retry", 
									 SCREEN_GAME, GAME_LOSE, 
									 BUTTON_CENTER, SCREEN_HEIGHT/2 + GPANEL_STANDARD_HEIGHT/2 - 120 - 100));
	buttonList.push_back(new CButton("menu_end", 
									 SCREEN_GAME, GAME_LOSE, 
									 BUTTON_CENTER, SCREEN_HEIGHT/2 + GPANEL_STANDARD_HEIGHT/2 - 120));
}

void Game::ButtonAction(CButton* button)
{
	string type = button->getType();
	// TITLE SCREEN
	switch(button->getScreen())
	{
	case SCREEN_MENU:
		switch(button->getState())
		{
		case MENU_TITLE:		
			if(type == "title_startgame")	EnterScenarioSel();
			if(type == "title_helpfile")	EnterHelpFile();
			if(type == "title_quit")		done = true;
			break;
		case MENU_HELPFILE:
			if(type == "menu_back")			EnterTitleScreen();
			if(type == "arrow_left")		ChangeHelpFilePage(-1);
			if(type == "arrow_right")		ChangeHelpFilePage(1);
			break;
		case MENU_SCENESEL:
			if(type == "arrow_up")			ChangeScenario(-1);
			if(type == "arrow_down")		ChangeScenario(1);
			if(type == "menu_enter")		EnterLevelSel();
			if(type == "menu_back")			EnterTitleScreen();
			break;		
		case MENU_LEVELSEL:
			if(type == "arrow_left")		ChangeLevel(-1);
			if(type == "arrow_right")		ChangeLevel(1);
			if(type == "menu_start")		EnterGame();
			if(type == "menu_back")			EnterScenarioSel();
			break;
		}
		break;
	case SCREEN_GAME:
		switch(button->getState())
		{
		case GAME_PREP:
			if(type == "menu_start")		GameContPrep();
			break;
		case GAME_PAUSED:
			if(type == "menu_retry")		EndGame(GAMEEND_RETRY);
			if(type == "menu_end")			EndGame(GAMEEND_QUIT);
			if(type == "menu_quit")			done = true;
			break;
		case GAME_FINISHLVL:
			if(type == "menu_nextlevel")	NextLevel();
			break;
		case GAME_FINISHSCN:
			if(type == "menu_mainmenu")		EndGame(GAMEEND_QUIT);
			break;
		case GAME_LOSE:
			if(type == "menu_retry")		EndGame(GAMEEND_RETRY);
			if(type == "menu_end")			EndGame(GAMEEND_QUIT);
			break;

		}
		break;
	}
}