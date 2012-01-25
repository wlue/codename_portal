
#ifndef _MAIN_H
#define _MAIN_H

// go around a MS Visual C++ 6 truncation warning bug with STL
// #pragma warning(disable: 4786)
// #pragma warning(disable: 4710)
// #pragma warning(disable: 4503)

// include standard libraries
#include <iostream>		// input and output to console (Debug messages)
#include <sstream>		// string stream: used to convert strings to other data types
#include <fstream>		// file stream: used to read/write files

#include <string>		// STL strings
#include <vector>		// STL vectors: lists objects
#include <map>			// STL maps: lists objects as data pairs
#include <cmath>		// math functions

#include <ctime>		// time functions: used to determine seconds
#include <algorithm>	// STL algorithm: used with vector/map

using namespace std;

// include SDL libraries
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "SDL_gfxPrimitives.h"
#include "SDL_rotozoom.h"

#define SHOW_LOADING_MESSAGE

// Forward decleration of classes
class Game;
class GScenario;
class GLevel;
class GMapLoader;
class ImageHandler;
class Timer;
class Image;
class ImageState;
class Client;
class CVisual;
class CObject;
class CTile;
class CTerrain;
class CDynamic;
class CMovable;
class CUnit;
class CPlayer;
class CTurret;
class CProjectile;
class CProjPortal;
class CPortal;
class CScreenObj;
class CButton;

typedef vector<SDL_Surface*> surface_vector;
typedef map<string, string> string_map;


// CONSTANTS 
const double pi = 3.14159265358979323846;
const string version = "1.0";
const string date = "September 8th, 2007";

const int
	FPS				= 40,					// maximum frame rate
	SCREEN_BPP		= 32,					// screen bits per pixel
	SHOW_FPS		= 1,					// show frame rate on screen?

	SOUND_FREQ		= 22050,				// sound bit rate
	SOUND_FORMAT	= MIX_DEFAULT_FORMAT,	// sound format
	SOUND_CHANNELS	= 2,					// number of channels in sound
	SOUND_CHUNKSIZE = 1024,					// chunk size

	TILE_WIDTH		= 32,					// width of generic tile
	TILE_HEIGHT		= 32,					// height of generic tile
	PORTAL_LENGTH	= 96,					// length of portal
	PORTAL_WIDTH	= 42,					// width of portal
	TURRET_DMG		= 7,					// amount of health player loses when shot by turret

	// directional bits
	NORTH	= 1,	SOUTH	= 2,
	EAST	= 4,	WEST	= 8,

	// autojoining methods
	AJ_NONE		= 0,
	AJ_16STATE	= 1,
	AJ_47STATE	= 2,
	AJ_161STATE	= 3,
	AJ_256STATE = 4,

	// autojoining bit flags
	AJ_N	= 1,	AJ_NE	= 2,
	AJ_E	= 4,	AJ_SE	= 8,
	AJ_S	= 16,	AJ_SW	= 32,
	AJ_W	= 64,	AJ_NW	= 128,

	PLAYER_MAXHEALTH = 100,					// maximum player health
	PLAYER_KEY_PAUSE	= SDLK_q,
	PLAYER_KEY_ENTER	= SDLK_w,			// player key config
	PLAYER_KEY_LEFT		= SDLK_a,
	PLAYER_KEY_RIGHT	= SDLK_d,
	PLAYER_KEY_JUMP		= SDLK_SPACE,

	ANGLE_PRECISION	= 90,					// angle precision used

	IMGSET_USEANGLES	= 0x01,				// does this image state generate angles?
	IMGSET_REWIND		= 0x02,				// will this image state rewind when animating?
	IMGSET_NOSMOOTH		= 0x04;				// use non-smooth methods of rotation generation

// GAME CONSTANTS
const int
	GAME_HELPFILE_PAGES = 6,

	// game states
	SCREEN_MENU		= 0,	// menu screen
	SCREEN_GAME		= 1,	// game screen

	MENU_TITLE		= 0,	// title screen
	MENU_HELPFILE	= 1,	// help file screen(s)
	MENU_SCENESEL	= 2,	// scenerio selection menu
	MENU_LEVELSEL	= 3,	// level selection menu

	GAME_PLAYING	= 0,	// playing screen (playing the game)
	GAME_PAUSED		= 1,	// paused screen
	GAME_PREP		= 2,	// preparation display box
	GAME_FINISHLVL	= 3,	// level finish display box
	GAME_FINISHSCN	= 4,	// scenario finish display box
	GAME_LOSE		= 5,	// game lose display box, with retry button

	// Game-end situations
	GAMEEND_WIN		= 0,	// player has won
	GAMEEND_RETRY	= 1,	// player has pressed "Retry"
	GAMEEND_QUIT	= 2,	// player has quit the game

	// Game-lose situations
	GAMELOSE_DIE	= 0,	// player has died
	GAMELOSE_TIME	= 1,	// time is up

	// menu screen constants

	BUTTON_CENTER = -1,
	BUTTON_LEFT = -2,
	BUTTON_RIGHT = -3,
	BUTTON_TOP = -4,
	BUTTON_BOTTOM = -5,

	HUD_PANEL_STATMARGIN	= 230,		// left distance to start drawing text in player hud
	HUD_PANEL_HEIGHT		= 76,		// height of hud panel

	GPANEL_STANDARD_HEIGHT	= 400,		// display panel dimensions
	GPANEL_STANDARD_WIDTH	= 500,
	GPANEL_PAUSE_HEIGHT		= 420,
	GPANEL_PAUSE_WIDTH		= 437,

	MENU_TITLE_TOP			= 250,		// first button location on title screen
	MENU_TITLE_INTERVAL		= 120,		// interval of buttons

	MENU_SCENE_IMG_TOP		= 200,		// scene selection box image location top
	MENU_SCENE_IMG_LEFT		= 140,
	MENU_SCENE_IMG_HEIGHT	= 300,		// scene selection box image dimensions
	MENU_SCENE_IMG_WIDTH	= 300,
	MENU_SCENE_MARGIN_LEFT	= 460,		// margin drawing text on right of img box

	MENU_MARGIN_TOP			= 130,		// general top distance to start drawing text
	MENU_MARGIN_BOTTOM		= 0,
	MENU_MARGIN_LEFT		= 20,
	MENU_MARGIN_RIGHT		= 20;

// PHYSICS CONSTANTS
const double
	GRAVITY				= 30,		// pixels per second acceleration [down]
	JUMP_FORCE			= 13,		// amount of force excerted when player jumps

	FRICTION			= 3.0,		// velocity reduced per frame while on a surface
	TERMINAL_VEL		= 50,		// terminal velocity: maximum velocity an object can move at in one direction
	MOVABLE_AIR_DEACCEL = 0.2,		// percent of effective movement while in air
	PLAYER_ACCEL		= 2,		// player's acceleration speed while moving left/right
	PLAYER_MAXSPEED		= 8;		// player's maximum running speed

// NAME CONSTANTS
const string
	GAME_NAME			= "CODENAME: Portal",
	IMAGE_DIRECTORY		= "rsc/images",				// directory of images
	SOUND_DIRECTORY		= "rsc/sounds",				// directory of sounds
	LEVEL_DIRECTORY		= "rsc/levels",				// directory of scenarios/levels
	SCENARIO_INDEX		= "rsc/levels/index.txt";	// filename of index used to read scenarios

// DYNAMIC CONSTANTS
extern int
	SCREEN_WIDTH,	// size of the screen in pixels
	SCREEN_HEIGHT,
	MAP_WIDTH,		// size of the map in pixels
	MAP_HEIGHT,
	map_maxx,		// size of the map in tiles
	map_maxy;

extern SDL_Surface* screen;
extern SDL_Event event;
extern Uint8* keys;
extern Uint32 counter;
extern SDL_Rect surfrect;	// global rect used to define position
extern SDL_Rect surfclip;	// global rect used to define clipping
extern bool done;			// is the game finished excecuting?  when true, game closes

// Engine handling
extern Game game;
extern ImageHandler* imgHandler;

// global lists of objects
extern vector<CObject*>			deletion;
extern vector<CObject*>			listobjs;
extern vector<CTile*>			listtil;
extern vector<CTerrain*>		listter;
extern vector<CDynamic*>		listdyn;
extern vector<CMovable*>		listmov;
extern vector<CUnit*>			listunt;
extern vector<CPlayer*>			listply;
extern vector<CTurret*>			listtrt;
extern vector<CProjectile*>		listprj;
extern vector<CProjPortal*>		listppt;
extern vector<CPortal*>			listptl;
extern vector<GScenario*>		listscn;

// global maps of data (fonts, colours, sounds)
extern map<string, TTF_Font*> fonts;
extern map<string, SDL_Color*> fontColors;
extern map<string, Mix_Chunk*> sfx;

extern void DeleteObj(CObject* obj);	// sends object into deletion vector
extern void PlaySFX(string sfxname);	// play a sound effect once to the user

// Fast surface blitting
extern void ApplySurface(SDL_Surface*, int, int);
extern void ApplySurface_Rect(SDL_Surface*, int, int, int, int, int, int);
extern void ApplySurface_Screen(SDL_Surface*, int, int, int, int, int, int);
extern SDL_Surface* load_image(string);
extern SDL_Surface* set_mask(SDL_Surface*, int, int, int);
extern SDL_Surface* roto_mask(SDL_Surface*);
extern SDL_Color* getSDLColor(int r, int g, int b);
extern SDL_Surface* TTF_DrawText(string fname, string text, string cname, int flags);
extern SDL_Surface* TTF_DrawText_Blended(string fname, string text, string cname, int flags);

// Does this file exist?
extern bool fileExists(string);

// angle/distance calculations
extern int normAngle(int angle);				// normalize angle
extern int mirrorAngle(int angle, int div);		// get mirror image of angle in a given anchor
extern int angle2dir(int angle);				// convert angle to bit direction
extern int dir2angle(int dir);					// convert bit direction to angle
extern int getangle(double x1, double y1, double x2, double y2);			// get the angle between two coordinates
extern void getStep(double& nx, double& ny, double dist, double angle);		// step in a distance given an angle
extern void getStepDist(double& dx, double& dy, double dist, double angle);	// get the x and y step needed given dist and angle
extern int dirTurn(int dir, int turn);	// turn a direction given an angle
extern int dirstep(int dir1, int dir2); // step positive, or negative from dir1 to dir2
extern int dirstepDist(int dir1, int dir2); // closest distance from dir1 to dir2
extern int tileDist(int x1, int y1, int x2, int y2);	// real distance with tiles
extern double realDist(double x1, double y1, double x2, double y2);	// real distance with coordinates
extern string sec2clock(int seconds);

// randomizing functions
extern double randReal();
extern double randReal(double, double);
extern int rand(int);
extern int rand(int, int);
extern bool prob(int);

extern CTile* locateTile(int tx, int ty);
extern void getLocationStep(int& x, int& y, int dir, int steps);
extern CTile* getTileStep(CTile* tile, int dir, int steps);

// mathematical statistic functions
template <class U> inline U maxVal(U a, U b) { return a > b ? a : b; }
template <class U> inline U minVal(U a, U b) { return a < b ? a : b; }
template <class U> inline U lim(U val, U lo, U hi) { return maxVal(lo, minVal(hi, val)); }
template <class U> inline U floorVal(U val, U mul) { return val - val % mul; }
template <class U> inline U ceilVal(U val, U mul) { return val + val % mul; }
inline double round(double num) { return floor(num+0.5); }

// conversion between degrees-radians
inline double deg2rad(double degree) { return degree * pi/180; }
inline double rad2deg(double radians) { return radians * 180/pi; }

// type-safe type cast checking
template <class U, class T> 
inline bool isType(T* obj)
{ 
	U A = dynamic_cast<U> (obj); 
	return A != NULL; 
}

// is an item in this vector?
template <class U> inline bool inVector(vector<U>& list, U item)
{
	typename vector<U>::iterator vItr = find(list.begin(), list.end(), item);
	if(*vItr == NULL) return false;
	return true;
}

// get the number index of an item in a vector
template <class U>
int itemIndex(vector<U> list, U item)	
{
	for(int i=0; i<int(list.size()); i++)
		if(list.at(i) == item)
			return i;
	return -1;
}

// get the next item in a vector given a starting point and direction
template <class U>
U nextItem(vector<U> list, U start, int amp)	
{
	int size = int(list.size());
	int nextIndex = itemIndex(list, start) + amp;
	while(nextIndex < 0) nextIndex += size;
	while(nextIndex >= size) nextIndex -= size;
	return list.at(nextIndex);
}

// tile location -> pixel location conversions
inline int xloc2tile(double t)	{ return floorVal<int>((int) t, TILE_WIDTH) / TILE_WIDTH; }
inline int yloc2tile(double t)	{ return floorVal<int>((int) t, TILE_HEIGHT) / TILE_HEIGHT; }
inline int xtile2loc(int t)		{ return (t*TILE_WIDTH + TILE_WIDTH/2); }
inline int ytile2loc(int t)		{ return (t*TILE_HEIGHT + TILE_HEIGHT/2); }
inline double sign(double val) { return (val>0) ? 1 : ((val<0) ? -1 : 0); }

// convert from a string to a value
template <class U>
U str2val(string s)
{
	stringstream ss; U val;
	ss << s;	ss >> val;
	return val;
}

// Sets in string "a=b": a, b; given split as "="
template <class A, class B>
void GetSides(A& left, B& right, string text, string split)
{
	string strleft = text.substr(0, text.find(split));
	string strright = text.substr(text.find(split)+1);
	left = str2val<A>(strleft);
	right = str2val<B>(strright);
}

inline void GetStringSides(string& left, string& right, string text, string split)
{
	left = text.substr(0, text.find(split));
	right = text.substr(text.find(split)+1);
}

// splits a text string into multiple values given the split character
inline void SplitValues(vector<string>& valList, string text, char split)
{
	string::iterator textItr;
	string currText = "";
	for(textItr = text.begin(); textItr != text.end(); ++textItr)
	{
		char s = *textItr;
		if(s == split && currText != "")
		{
			valList.push_back(currText);
			currText = "";
		}
		else
			currText += s;
	}
	if(currText != "") valList.push_back(currText);
}


// removes an element from an array, returning true when successful
template <class U>
bool RemoveElement(vector<U>& list, U item)
{
	typename vector<U>::iterator Itr = find(list.begin(), list.end(), item);
	if(Itr != list.end())
	{
		list.erase(Itr);
		return true;
	}
	return false;
}

// convert from a value to a string
template <class U>
string val2str(U val)
{
	string str;	stringstream ss;
	ss << val;	ss >> str;
	return str;
}

// get the distance between to coordinates using the Pythagorean theorem
inline double distance(double x1, double y1, double x2, double y2)
{
	return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

// optimization of distance used to compare distances
inline double distance_bare(double x1, double y1, double x2, double y2)
{
	return (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
}

// returns what frame per counter in a multiple of m, used in animation
inline int frameState(int counter, int frame, int multiple) 
{ 
	return (floorVal(counter, multiple)%(frame*multiple))/multiple;
}

// returns the direction from one value to another
inline double sign_difference(double cmp, double value)
{
	return (cmp < 0) ? value : ((cmp > 0) ? -value : 0);
}

inline string angle2dirstate(int angle)
{
	if(10 < angle && angle < 170)			return "-left";
	else if(190 < angle && angle < 350)		return "-right";
	else									return "";
}

// condition for either iterating up or down through a value in a range
template <class U> inline bool inRangeOf(U val, U range)
{
	return range>0 ? val<range : val>range;
}


// COLLISION
namespace collision
{
	// box->box collision
	// extern inline bool box(double Ax, double Ay, int Aw, int Ah, double Bx, double By, int Bw, int Bh);
	extern bool box(double Ax, double Ay, int Aw, int Ah, double Bx, double By, int Bw, int Bh);
	// circle->circle collision
	extern inline bool circle(double Ax, double Ay, int Ar, double Bx, double By, int Br);
	// circle->box collision
	extern bool circlebox(double Ax, double Ay, int Ar, double Bx, double By, int Bw, int Bh);
}

// PORTAL FUNCTIONS
namespace portal
{
	// calculate an object's new angle given the entrance and exit portals and original angle
	extern int calcObjAngle(CPortal* enter, CPortal* exit, int angle, int angleDiff);
	// calculate the relative distance from the exit portal given location from entrance portal
	extern void calcStepDist(double& dx, double& dy, CPortal* enter, CPortal* exit, CDynamic* obj, int angleDiff);
	// calculate the new velocity when entering portal
	extern void calcStepVel(double& vx, double& vy, CPortal* enter, CPortal* exit, CDynamic* obj, int angleDiff);
	// is an object in a portal?
	extern bool inArea(CObject* obj, double xloc, double yloc);
	// is an object in the portal's area where collision takes into account?
	extern bool inAreaOfEffect(CObject* obj, double xloc, double yloc);
	// is an object inside the base of a portal?
	extern CPortal* inWarpArea(CDynamic* obj, double xloc, double yloc);
}

#endif
