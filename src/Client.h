
#ifndef _CLIENT_H
#define _CLIENT_H

#include "main.h"

// The user playing the game.  Independant from controlee
class Client
{
private:
	SDL_Rect camera, screen_loc;
	CPlayer* control;
	CDynamic* eye;

	map<string, SDL_Surface*>	dynSurfaces;
	void SetDynSurface(string name, SDL_Surface* newsurface);

	void Initialize();

public:
	Client();
	void SetScreenProperties(int x, int y, int w, int h);	// x, y, w, h
	
	CPlayer* getControl() { return control; }
	CDynamic* getEye() { return eye; }
	int getSX() { return screen_loc.x; }
	int getSY() { return screen_loc.y; }
	int getSW() { return screen_loc.x + camera.w; }
	int getSH() { return screen_loc.y + camera.h; }
	int getCameraX() { return camera.x; }
	int getCameraY() { return camera.y; }
	bool inView(CObject* obj);

	void setTo(CPlayer* obj);
	void removeFrom();

	void setControl(CPlayer* obj);
	void setEye(CDynamic* obj);
	void removeControl();
	void removeEye();

	void SetupPlayer();
	void PlayerDoorCheck();
		
	void UpdateCamera();
	void HandleEvents();
	void HandleDynamicEvents();

	void DeathCheck();
	void HandlePlayerEvents();
	void HandlePlayerDynEvents();

	void UpdateHUD();
	void draw();
	void drawHUD();
};

#endif