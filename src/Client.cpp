
#include "Client.h"
#include "Game.h"

#include "CTile.h"
#include "CDoor.h"
#include "CObject.h"
#include "CDynamic.h"
#include "CUnit.h"
#include "CTerrain.h"
#include "CPlayer.h"
#include "CTurret.h"
#include "CPortal.h"
#include "CProjectile.h"
#include "CButton.h"
#include "GScenario.h"
#include "GLevel.h"

Client::Client()
{
	Initialize();
}

void Client::Initialize()
{
	// Set initial camera position/dimensions
	removeControl();
	camera.x = camera.y = 0;
	camera.w = SCREEN_WIDTH;
	camera.h = SCREEN_HEIGHT - TILE_HEIGHT*2; // height is reduced by panel hud's height
	UpdateHUD();
}

void Client::SetScreenProperties(int x, int y, int w, int h)
{
	// Set screen dimensions/displacement
	screen_loc.x = x;	screen_loc.y = y;
	camera.w = w;		camera.h = h;
}

void Client::SetupPlayer()
{
	// If no reference made, create new player reference
	if(!control) setTo(new CPlayer(this));
	control->Reset();

	// Locate player to entrance door in the level
	vector<CTile*>::iterator a;
	for(a=listtil.begin(); a!=listtil.end(); ++a)
	{
		CDoor* door = dynamic_cast<CDoor*> (*a);
		if(door != NULL && door->isEntrance())
			control->setLoc(door->getX(), door->getY() + door->getH()/2 - control->getH()/2);
	}
}

void Client::PlayerDoorCheck()
{
	// Player wins when is at the exit door
	if(control && control->inDoor())
		game.EndGame(GAMEEND_WIN);
}

bool Client::inView(CObject* obj)
{
	// Is an object in the client's screen?
	if(((obj->getX() + obj->getW()) < camera.x) || ((obj->getX() - obj->getW()) > camera.x + camera.w))
		return false;
	if(obj->getY() + obj->getH() < camera.y || obj->getY() - obj->getW() > camera.y + camera.h)
		return false;

	return true;
}

void Client::setTo(CPlayer* obj)
{
	removeFrom();
	control = obj;
	eye = obj;
}

void Client::setControl(CPlayer* obj)
{
	removeControl();
	control = obj;
}

void Client::setEye(CDynamic* obj)
{
	removeEye();
	eye = obj;
}

void Client::removeFrom()
{
	removeControl();
	removeEye();
}

void Client::removeControl()
{
	control = NULL;
}

void Client::removeEye()
{
	eye = NULL;
}

void Client::UpdateCamera()
{

	CDynamic* look = getEye();
	if(look == NULL) return;

	// Set the camera's location to the area closest to the player's center
	camera.x = lim<int>(int(look->getX()) - camera.w/2, 0, MAP_WIDTH - camera.w);
	camera.y = lim<int>(int(look->getY()) - camera.h/2, 0, MAP_HEIGHT - camera.h);
}

void Client::HandleEvents()
{
	// Handle button events
	vector<CButton*>::iterator a;
	for(a=game.buttonList.begin(); a!=game.buttonList.end(); ++a)
		if((*a)->inScreen(game.gscreen, game.gstate))
			(*a)->onEvent(event.type);
}

void Client::HandleDynamicEvents()
{

}

void Client::HandlePlayerEvents()
{
	// Handle player input (keys, mouse)
	if(!control) return;

	if(event.type == SDL_MOUSEBUTTONDOWN)
	{
		control->Shoot(event.button.button);
	}

	else if(event.type == SDL_KEYDOWN)
	{
		switch(event.key.keysym.sym)
		{
		case PLAYER_KEY_JUMP:
			control->Jump();
			break;
		case PLAYER_KEY_ENTER:
			PlayerDoorCheck();
			break;
		case PLAYER_KEY_LEFT:
			if(!keys[PLAYER_KEY_RIGHT])
				control->setXAccel(-PLAYER_ACCEL);
			break;
		case PLAYER_KEY_RIGHT:
			if(!keys[PLAYER_KEY_LEFT])
				control->setXAccel(PLAYER_ACCEL);
			break;
		default:
			break;
		}
	}
	else if(event.type == SDL_KEYUP)
	{
		switch(event.key.keysym.sym)
		{
		case PLAYER_KEY_LEFT:
			control->setXAccel(0);
			if(keys[PLAYER_KEY_RIGHT])
				control->setXAccel(PLAYER_ACCEL);
			break;
		case PLAYER_KEY_RIGHT:
			control->setXAccel(0);
			if(keys[PLAYER_KEY_LEFT])
				control->setXAccel(-PLAYER_ACCEL);
			break;
		default:
			break;
		}
	}
}

void Client::HandlePlayerDynEvents()
{
	// Handle player mouse movement (direction)
	CDynamic* look = getEye();
	int ax = getSX() + (int(look->getX()) - getCameraX());
	int ay = getSY() + (int(look->getY()) - getCameraY());
	int px,  py;
	SDL_PumpEvents();
	SDL_GetMouseState(&px, &py);
	int angle = getangle(ax, ay, px, py);
	control->setDir(angle);
	// Face the player to the mouse pointer
}

void Client::DeathCheck()
{
	// If the player has died, then end the game
	if(control == NULL) return;
	UpdateHUD();
	if(control->getHealth() <= 0) {
		game.LoseLevel(GAMELOSE_DIE);
	}
}

void Client::SetDynSurface(string name, SDL_Surface* newsurface)
{
	SDL_Surface* surface = dynSurfaces[name];
	SDL_FreeSurface(surface);
	dynSurfaces[name] = newsurface;
}

void Client::UpdateHUD()
{
	if(control == NULL) return;
	// Update the panel, health, level, and time images
	dynSurfaces["panel"]	= imgHandler->getImage("hud")->getImage("panel", 0, false);
	SetDynSurface("health", TTF_DrawText_Blended("htext", "Health: " + val2str(control->getHealth()),	"black", 0));
	SetDynSurface("time",	TTF_DrawText_Blended("htext", "Time Left: " + sec2clock(game.timeLeft),		"black", 0));
	SetDynSurface("level",	TTF_DrawText_Blended("htext", 
		"Level " + val2str(itemIndex(game.scenario->getLevels(), game.level) + 1) + " of " + 
		val2str(game.scenario->getLevels().size()),		"black", 0));
}

void Client::drawHUD()
{
	// Drawing Heads-Up Display
	if(!control) return;
	ApplySurface(dynSurfaces["panel"],		0, screen->h - dynSurfaces["panel"]->h);
	ApplySurface(dynSurfaces["health"],		HUD_PANEL_STATMARGIN, screen->h - 50);
	ApplySurface(dynSurfaces["time"],		10, screen->h - 70);
	ApplySurface(dynSurfaces["level"],		10, screen->h - 35);
}

void Client::draw()
{
	// Update camera position, and blit all objects on the map in order
	UpdateCamera();
	SDL_FillRect(screen, &screen_loc, SDL_MapRGB(screen->format, 0xcc, 0xcc, 0xcc));

	vector<CTile*>::iterator a;
	for(a=listtil.begin(); a!=listtil.end(); ++a)
		(*a)->draw(this);

	vector<CPortal*>::iterator b;
	for(b=listptl.begin(); b!=listptl.end(); ++b)
		(*b)->drawLedge(this);

	vector<CPlayer*>::iterator c;
	for(c=listply.begin(); c!=listply.end(); ++c)
		(*c)->draw(this);

	vector<CTurret*>::iterator d;
	for(d=listtrt.begin(); d!=listtrt.end(); ++d)
		(*d)->draw(this);
	
	vector<CTerrain*>::iterator e;
	for(e=listter.begin(); e!=listter.end(); ++e)
		(*e)->draw(this);

	vector<CProjectile*>::iterator f;
	for(f=listprj.begin(); f!=listprj.end(); ++f)
		(*f)->draw(this);

	vector<CPortal*>::iterator z;
	for(z=listptl.begin(); z!=listptl.end(); ++z)
		(*z)->drawBase(this);

	// Draw the HUD last
	drawHUD();
}
